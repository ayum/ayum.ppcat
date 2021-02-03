#include "picker.hpp"
#include "logging.hpp"

#include <napi.h>
#include <string>
#include <string_view>

using namespace ppcat;
using namespace nlohmann;

static std::unique_ptr<backend::picker> picker;

namespace {

Napi::Object convert(Napi::Env env, const nlohmann::json &data) {
    auto convert_impl = [](auto &convert_ref, Napi::Env &env, const nlohmann::json &data) -> Napi::Value {
        switch (data.type()) {
        case json::value_t::string:
        {
            return Napi::String::New(env, data.get<std::string>());
        }
        break;
        case json::value_t::array:
        {
            Napi::Array result = Napi::Array::New(env, data.size());
            size_t n = 0;
            for (auto it = data.begin(); it != data.end(); ++n, ++it) {
                result.Set(n, convert_ref(convert_ref, env, it.value()));
            }
            return result;
        }
        break;
        case json::value_t::object:
        {
            Napi::Object result = Napi::Object::New(env);
            for (auto it = data.begin(); it != data.end(); ++it) {
                result.Set(it.key(), convert_ref(convert_ref, env, it.value()));
            }
            return result;
        }
        break;
        default:
            common::log::critical_throw("Picked json data is not valid. Use trace log level to inspect it");
        }

        common::log::critical_throw("Have reached unreachable");
    };

    Napi::Value ret = convert_impl(convert_impl, env, data);
    return ret.As<Napi::Object>();
}

}

static Napi::Object pick(const Napi::CallbackInfo& info) {
    const Napi::Value &&arg = info[0];
    if (not arg.IsString()) {
        throw Napi::Error::New(info.Env(), "First argument of pick method must be string");
    }
    std::string input = info[0].As<Napi::String>();

    json data = picker->pick(input);

    Napi::Object ret = convert(info.Env(), data);
    return ret;
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    picker.reset();
    exports["pick"] = Napi::Function::New(env, pick);
    return exports;
}

NODE_API_MODULE(ppcat, Init)
