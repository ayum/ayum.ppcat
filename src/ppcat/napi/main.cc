#include <assert.h>
#include <napi.h>
#include <string>
#include <string_view>

static Napi::Value ppcat(const Napi::CallbackInfo& info) {
    Napi::Value ret = info[0];
    return ret;
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports["ppcat"] = Napi::Function::New(env, ppcat);
  return exports;
}

NODE_API_MODULE(ppcat, Init)
