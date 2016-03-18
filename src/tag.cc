#include "tag.h"

#include <errno.h>
#include <string.h>

#include <node_buffer.h>

#include "taglib.h"
#include "bufferstream.h"

using namespace node_taglib;
using namespace v8;
using namespace node;

namespace node_taglib {

NAN_MODULE_INIT(Tag::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>();
    tpl->SetClassName(Nan::New("Tag").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    Nan::SetPrototypeMethod(tpl, "save", AsyncSaveTag);
    Nan::SetPrototypeMethod(tpl, "saveSync", SyncSaveTag);
    Nan::SetPrototypeMethod(tpl, "isEmpty", IsEmpty);
    
    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("title").ToLocalChecked(), GetTitle, SetTitle);
    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("album").ToLocalChecked(), GetAlbum, SetAlbum);
    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("comment").ToLocalChecked(), GetComment, SetComment);
    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("artist").ToLocalChecked(), GetArtist, SetArtist);
    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("track").ToLocalChecked(), GetTrack, SetTrack);
    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("year").ToLocalChecked(), GetYear, SetYear);
    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("genre").ToLocalChecked(), GetGenre, SetGenre);
    
    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("Tag").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
    Nan::SetMethod(target, "tag", AsyncTag);
    Nan::SetMethod(target, "tagSync", SyncTag);
}

Tag::Tag(TagLib::FileRef * ffileRef) : tag(ffileRef->tag()), fileRef(ffileRef) { }

Tag::~Tag() {
    if (fileRef)
        delete fileRef;
    fileRef = NULL;
    tag = NULL;
}

inline Tag * unwrapTag(const Nan::PropertyCallbackInfo<v8::Value>& info) {
  return Nan::ObjectWrap::Unwrap<Tag>(info.Holder());
}
inline Tag * unwrapTag(const Nan::PropertyCallbackInfo<void>& info) {
  return Nan::ObjectWrap::Unwrap<Tag>(info.Holder());
}


NAN_GETTER(Tag::GetTitle) {
    info.GetReturnValue().Set(TagLibStringToString(unwrapTag(info)->tag->title()));
}
NAN_SETTER(Tag::SetTitle) {
    unwrapTag(info)->tag->setTitle(NodeStringToTagLibString(value));  
}

NAN_GETTER(Tag::GetArtist) {
    info.GetReturnValue().Set(TagLibStringToString(unwrapTag(info)->tag->artist()));
}
NAN_SETTER(Tag::SetArtist) {
    unwrapTag(info)->tag->setArtist(NodeStringToTagLibString(value));  
}

NAN_GETTER(Tag::GetAlbum) {
    info.GetReturnValue().Set(TagLibStringToString(unwrapTag(info)->tag->album()));
}
NAN_SETTER(Tag::SetAlbum) {
    unwrapTag(info)->tag->setAlbum(NodeStringToTagLibString(value));  
}

NAN_GETTER(Tag::GetComment) {
    info.GetReturnValue().Set(TagLibStringToString(unwrapTag(info)->tag->comment()));
}
NAN_SETTER(Tag::SetComment) {
    unwrapTag(info)->tag->setComment(NodeStringToTagLibString(value));  
}

NAN_GETTER(Tag::GetTrack) {
    info.GetReturnValue().Set(unwrapTag(info)->tag->track());
}
NAN_SETTER(Tag::SetTrack) {
    unwrapTag(info)->tag->setTrack(value->IntegerValue());  
}

NAN_GETTER(Tag::GetYear) {
    info.GetReturnValue().Set(unwrapTag(info)->tag->year());
}
NAN_SETTER(Tag::SetYear) {
    unwrapTag(info)->tag->setYear(value->IntegerValue());  
}

NAN_GETTER(Tag::GetGenre) {
    info.GetReturnValue().Set(TagLibStringToString(unwrapTag(info)->tag->genre()));
}
NAN_SETTER(Tag::SetGenre) {
    unwrapTag(info)->tag->setGenre(NodeStringToTagLibString(value));  
}

NAN_METHOD(Tag::IsEmpty) {
    Tag *t = ObjectWrap::Unwrap<Tag>(info.Holder());
    info.GetReturnValue().Set(t->tag->isEmpty());
}

NAN_METHOD(Tag::SyncSaveTag) {
    Tag *t = ObjectWrap::Unwrap<Tag>(info.Holder());
    assert(t->fileRef);
    bool success = t->fileRef->save();
    if (success)
        info.GetReturnValue().SetUndefined();
    else
        Nan::ThrowError("Failed to save file.");
        //TODO: filename
}

NAN_METHOD(Tag::AsyncSaveTag) {
    if (info.Length() >= 1 && !info[0]->IsFunction()) {
        Nan::ThrowError("Expected callback function as first argument");
        return;
    }

    Local<Function> callback = Local<Function>::Cast(info[0]);

    Tag *t = ObjectWrap::Unwrap<Tag>(info.Holder());

    AsyncBaton *baton = new AsyncBaton;
    baton->request.data = baton;
    baton->tag = t;
    baton->callback.Reset(callback);
    baton->error = 1;

    uv_queue_work(uv_default_loop(), &baton->request, Tag::AsyncSaveTagDo, (uv_after_work_cb)Tag::AsyncSaveTagAfter);

    info.GetReturnValue().SetUndefined();
}

void Tag::AsyncSaveTagDo(uv_work_t *req) {
    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    assert(baton->tag->fileRef);
    baton->error = !baton->tag->fileRef->save();
}

void Tag::AsyncSaveTagAfter(uv_work_t *req) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    if (baton->error) {
        Local<Object> error = Nan::New<Object>();
        error->Set(Nan::New("message").ToLocalChecked(), Nan::New("Failed to save file").ToLocalChecked());
        error->Set(Nan::New("path").ToLocalChecked(), Nan::New(baton->tag->fileRef->file()->name()).ToLocalChecked());
        Handle<Value> argv[] = { error };
        Nan::Call(Nan::New(baton->callback), Nan::GetCurrentContext()->Global(), 1, argv);
    }
    else {
        Handle<Value> argv[] = { Nan::Null() };
        Nan::Call(Nan::New(baton->callback), Nan::GetCurrentContext()->Global(), 1, argv);
    }

    //baton->callback.Dispose();
    delete baton;
}

NAN_METHOD(Tag::SyncTag) {
    TagLib::FileRef *f = 0;
    int error = 0;

    if (info.Length() >= 1 && info[0]->IsString()) {
        String::Utf8Value path(info[0]->ToString());
        if ((error = CreateFileRefPath(*path, &f))) {
            Local<String> fn = String::Concat(info[0]->ToString(), Nan::New(": ", -1).ToLocalChecked());
            Nan::ThrowError(String::Concat(fn, ErrorToString(error)));
            return;
        }
    } else if (info.Length() >= 1 && Buffer::HasInstance(info[0])) {
        if (info.Length() < 2 || !info[1]->IsString()) {
            Nan::ThrowError("Expected string 'format' as second argument");
            return;
        }

        if ((error = CreateFileRef(new BufferStream(info[0]->ToObject()), NodeStringToTagLibString(info[1]->ToString()), &f))) {
            Nan::ThrowError(ErrorToString(error));
            return;
        }
    } else {
        Nan::ThrowError("Expected string or buffer as first argument");
        return;
    }

    Tag * tag = new Tag(f);
    Local<Object> inst = Nan::NewInstance(Nan::New(constructor())).ToLocalChecked();
    tag->Wrap(inst);

    info.GetReturnValue().Set(inst);
}

NAN_METHOD(Tag::AsyncTag) {
    if (info.Length() < 1) {
        Nan::ThrowError("Expected string or buffer as first argument");
        return;
    }

    if (info[0]->IsString()) {
        if (info.Length() < 2 || !info[1]->IsFunction()) {
            Nan::ThrowError("Expected callback function as second argument");
            return;
        }
    } else if (Buffer::HasInstance(info[0])) {
        if (info.Length() < 2 || !info[1]->IsString()) {
            Nan::ThrowError("Expected string 'format' as second argument");
            return;
        }
        if (info.Length() < 3 || !info[2]->IsFunction()) {
            Nan::ThrowError("Expected callback function as third argument");
            return;
        }
    } else {
        Nan::ThrowError("Expected string or buffer as first argument");
        return;
    }


    AsyncBaton *baton = new AsyncBaton;
    baton->request.data = baton;
    baton->path = 0;
    baton->tag = NULL;
    baton->error = 0;

    if (info[0]->IsString()) {
        String::Utf8Value path(info[0]->ToString());
        baton->path = strdup(*path);
        baton->callback.Reset(Local<Function>::Cast(info[1]));

    }
    else {
        baton->format = NodeStringToTagLibString(info[1]->ToString());
        baton->stream = new BufferStream(info[0]->ToObject());
        baton->callback.Reset(Local<Function>::Cast(info[2]));
    }

    uv_queue_work(uv_default_loop(), &baton->request, Tag::AsyncTagReadDo, (uv_after_work_cb)Tag::AsyncTagReadAfter);

    info.GetReturnValue().SetUndefined();
}

void Tag::AsyncTagReadDo(uv_work_t *req) {
    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    TagLib::FileRef *f;

    if (baton->path) {
        baton->error = node_taglib::CreateFileRefPath(baton->path, &f);
    }
    else {
        assert(baton->stream);
        baton->error = node_taglib::CreateFileRef(baton->stream, baton->format, &f);
    }

    if (baton->error == 0) {
        baton->tag = new Tag(f);
    }
}

void Tag::AsyncTagReadAfter(uv_work_t *req) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    if (baton->error) {
        Local<Object> error = Nan::New<Object>();
        error->Set(Nan::New("code").ToLocalChecked(), Nan::New(baton->error));
        error->Set(Nan::New("message").ToLocalChecked(), ErrorToString(baton->error));
        Handle<Value> argv[] = { error, Nan::Null() };
        Nan::Call(Nan::New(baton->callback), Nan::GetCurrentContext()->Global(), 2, argv);
    }
    else {
        Local<Object> inst = Nan::NewInstance(Nan::New(constructor())).ToLocalChecked();
        baton->tag->Wrap(inst);
        Handle<Value> argv[] = { Nan::Null(), inst };
        Nan::Call(Nan::New(baton->callback), Nan::GetCurrentContext()->Global(), 2, argv);
    }

    //baton->callback.Dispose();
    delete baton->path;
    delete baton;
}
}
