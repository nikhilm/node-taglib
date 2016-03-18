#ifndef NODE_TAGLIB_TAG_H
#define NODE_TAGLIB_TAG_H

#include <taglib.h>
#include <fileref.h>
#include <node.h>
#include <uv.h>
#include <sys/time.h>

#include <nan.h>
#include <nan_object_wrap.h>

namespace node_taglib {
class Tag : public Nan::ObjectWrap {

private:
    TagLib::Tag * tag;
    TagLib::FileRef *fileRef;

  //static v8::Persistent<v8::FunctionTemplate> pft;

public:
    static NAN_MODULE_INIT(Init);
    Tag(TagLib::FileRef * fileRef);
    ~Tag();
    
    static inline Nan::Persistent<v8::Function> & constructor() {
        static Nan::Persistent<v8::Function> my_constructor;
        return my_constructor;
    }

    static NAN_GETTER(GetTitle);
    static NAN_SETTER(SetTitle);
    
    static NAN_GETTER(GetArtist);
    static NAN_SETTER(SetArtist);
    
    static NAN_GETTER(GetAlbum);
    static NAN_SETTER(SetAlbum);
    
    static NAN_GETTER(GetYear);
    static NAN_SETTER(SetYear);
    
    static NAN_GETTER(GetComment);
    static NAN_SETTER(SetComment);
    
    static NAN_GETTER(GetTrack);
    static NAN_SETTER(SetTrack);
    
    static NAN_GETTER(GetGenre);
    static NAN_SETTER(SetGenre);
    
    static NAN_METHOD(IsEmpty);
    static NAN_METHOD(AsyncSaveTag);
    static NAN_METHOD(SyncSaveTag);
    static NAN_METHOD(SyncTag);
    static NAN_METHOD(AsyncTag);
    
    static void AsyncTagReadDo(uv_work_t *req);
    static void AsyncTagReadAfter(uv_work_t *req);
    static void AsyncSaveTagDo(uv_work_t *req);
    static void AsyncSaveTagAfter(uv_work_t *req);
};
}
#endif
