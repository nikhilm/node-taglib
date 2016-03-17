#ifndef NODE_TAGLIB_TAG_H
#define NODE_TAGLIB_TAG_H

#include <fileref.h>
#include <node.h>
#include <uv.h>
#include <node_object_wrap.h>
#include <sys/time.h>

namespace node_taglib {
class Tag : public node::ObjectWrap {
  TagLib::Tag * tag;
  TagLib::FileRef * fileRef;

  //static v8::Persistent<v8::FunctionTemplate> pft;

    public:
    static void Initialize(v8::Handle<v8::Object> target);
    Tag(TagLib::FileRef * fileRef);
    ~Tag();

    static void GetTitle(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetTitle(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void GetArtist(v8::Local< v8::String > property, const v8::PropertyCallbackInfo< v8::Value >& info);
    static void SetArtist(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void GetAlbum(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetAlbum(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void GetYear(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetYear(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void GetComment(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetComment(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void GetTrack(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetTrack(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

    static void GetGenre(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetGenre(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);

    void IsEmpty(const v8::FunctionCallbackInfo< v8::Value >& args);
    void AsyncSaveTag(const v8::FunctionCallbackInfo< v8::Value >& args);
    void SyncSaveTag(const v8::FunctionCallbackInfo< v8::Value >& args);
    void SyncTag(const v8::FunctionCallbackInfo< v8::Value >& args);
    void AsyncTag(const v8::FunctionCallbackInfo< v8::Value >& args);
    static void AsyncTagReadDo(uv_work_t *req);
    static void AsyncTagReadAfter(uv_work_t *req);
    static void AsyncSaveTagDo(uv_work_t *req);
    static void AsyncSaveTagAfter(uv_work_t *req);
};
}
#endif
