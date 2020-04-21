#include "picker.hpp"

#include <napi.h>
#include <string>
#include <string_view>

using namespace ppcat;
using namespace nlohmann;

static std::unique_ptr<backend::picker> picker;

static Napi::Value pick(const Napi::CallbackInfo& info) {
    const Napi::Value &&arg = info[0];
    if (not arg.IsString()) {
        throw Napi::Error::New(info.Env(), "First argument of pick method must be string");
    }
    std::string input = info[0].As<Napi::String>();

    json data = picker->pick(input);

    Napi::String ret = Napi::String::New(info.Env(), data.dump(2));
    return ret;
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    picker.reset();
    exports["pick"] = Napi::Function::New(env, pick);
    return exports;
}

NODE_API_MODULE(ppcat, Init)
