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

static Persistent<FunctionTemplate> TagTemplate;

void Tag::Initialize(Handle<Object> target)
{
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    TagTemplate = Persistent<FunctionTemplate>(isolate, FunctionTemplate::New(isolate));

    TagTemplate.Get(isolate)->InstanceTemplate()->SetInternalFieldCount(1);
    TagTemplate.Get(isolate)->SetClassName(String::NewSymbol("Tag"));

    NODE_SET_PROTOTYPE_METHOD(TagTemplate, "save", AsyncSaveTag);
    NODE_SET_PROTOTYPE_METHOD(TagTemplate, "saveSync", SyncSaveTag);
    NODE_SET_PROTOTYPE_METHOD(TagTemplate, "isEmpty", IsEmpty);

    TagTemplate.Get(isolate)->InstanceTemplate()->SetAccessor(String::New("title"), GetTitle, SetTitle);
    TagTemplate.Get(isolate)->InstanceTemplate()->SetAccessor(String::New("album"), GetAlbum, SetAlbum);
    TagTemplate.Get(isolate)->InstanceTemplate()->SetAccessor(String::New("comment"), GetComment, SetComment);
    TagTemplate.Get(isolate)->InstanceTemplate()->SetAccessor(String::New("artist"), GetArtist, SetArtist);
    TagTemplate.Get(isolate)->InstanceTemplate()->SetAccessor(String::New("track"), GetTrack, SetTrack);
    TagTemplate.Get(isolate)->InstanceTemplate()->SetAccessor(String::New("year"), GetYear, SetYear);
    TagTemplate.Get(isolate)->InstanceTemplate()->SetAccessor(String::New("genre"), GetGenre, SetGenre);

    target->Set(String::NewSymbol("Tag"), TagTemplate.Get(isolate)->GetFunction());
    NODE_SET_METHOD(target, "tag", AsyncTag);
    NODE_SET_METHOD(target, "tagSync", SyncTag);
}

Tag::Tag(TagLib::FileRef * ffileRef) : tag(ffileRef->tag()), fileRef(ffileRef) { }

Tag::~Tag() {
    if (fileRef)
        delete fileRef;
    fileRef = NULL;
    tag = NULL;
}

inline Tag * unwrapTag(const PropertyCallbackInfo<Value>& info) {
  return ObjectWrap::Unwrap<Tag>(info.Holder());
}

void Tag::GetTitle(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  info.GetReturnValue().Set(TagLibStringToString(unwrapTag(info)->tag->title()));
}

void Tag::SetTitle(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
  unwrapTag(info)->tag->setTitle(NodeStringToTagLibString(value));
}
void Tag::GetArtist(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
  info.GetReturnValue().Set(TagLibStringToString(unwrapTag(info)->tag->artist()));
}

void Tag::SetArtist(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
  unwrapTag(info)->tag->setArtist(NodeStringToTagLibString(value));
}

void Tag::GetAlbum(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
  info.GetReturnValue().Set(TagLibStringToString(unwrapTag(info)->tag->album()));
}

void Tag::SetAlbum(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
  unwrapTag(info)->tag->setAlbum(NodeStringToTagLibString(value));
}

void Tag::GetComment(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
  info.GetReturnValue().Set(TagLibStringToString(unwrapTag(info)->tag->comment()));
}

void Tag::SetComment(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
  unwrapTag(info)->tag->setComment(NodeStringToTagLibString(value));
}

void Tag::GetTrack(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
  info.GetReturnValue().Set(Integer::New(unwrapTag(info)->tag->track()));
}

void Tag::SetTrack(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
  unwrapTag(info)->tag->setTrack(value->IntegerValue());
}

void Tag::GetYear(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
  info.GetReturnValue().Set(Integer::New(unwrapTag(info)->tag->year()));
}

void Tag::SetYear(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
  unwrapTag(info)->tag->setYear(value->IntegerValue());
}

void Tag::GetGenre(Local<String> property, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
  info.GetReturnValue().Set(TagLibStringToString(unwrapTag(info)->tag->genre()));
}

void Tag::SetGenre(Local<String> property, Local<Value> value, const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
  unwrapTag(info)->tag->setGenre(NodeStringToTagLibString(value));
}

void Tag::IsEmpty(const FunctionCallbackInfo<Value> &args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  Tag *t = ObjectWrap::Unwrap<Tag>(args.This());
  args.GetReturnValue().Set(Boolean::New(t->tag->isEmpty()));
}

void Tag::SyncSaveTag(const FunctionCallbackInfo<Value> &args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  Tag *t = ObjectWrap::Unwrap<Tag>(args.This());
  assert(t->fileRef);
  bool success = t->fileRef->save();
  if (success)
      args.GetReturnValue().SetUndefined();
  else
      isolate->ThrowException(String::Concat(
                String::New("Failed to save file: "),
                String::New(t->fileRef->file()->name())
            ));
}

void Tag::SyncTag(const FunctionCallbackInfo<Value> &args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    TagLib::FileRef *f = 0;
    int error = 0;

    if (args.Length() >= 1 && args[0]->IsString()) {
        String::Utf8Value path(args[0]->ToString());
        if ((error = CreateFileRefPath(*path, &f))) {
            Local<String> fn = String::Concat(args[0]->ToString(), Local<String>::Cast(String::New(": ", -1)));
            isolate->ThrowException(String::Concat(fn, ErrorToString(error)));
            return;
        }
    }
    else if (args.Length() >= 1 && Buffer::HasInstance(args[0])) {
        if (args.Length() < 2 || !args[1]->IsString()) {
            isolate->ThrowException(String::New("Expected string 'format' as second argument"));
            return;
        }

        if ((error = CreateFileRef(new BufferStream(args[0]->ToObject()), NodeStringToTagLibString(args[1]->ToString()), &f))) {
            isolate->ThrowException(ErrorToString(error));
            return;
        }
    }
    else {
        isolate->ThrowException(String::New("Expected string or buffer as first argument"));
        return;
    }

    Tag * tag = new Tag(f);
    Handle<Object> inst = TagTemplate->InstanceTemplate()->NewInstance();
    tag->Wrap(inst);

    args.GetReturnValue().Set(inst);
}

void Tag::AsyncTag(const FunctionCallbackInfo<Value> &args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    

    if (args.Length() < 1) {
        isolate->ThrowException(String::New("Expected string or buffer as first argument"));
        return;
    }

    if (args[0]->IsString()) {
        if (args.Length() < 2 || !args[1]->IsFunction()) {
            isolate->ThrowException(String::New("Expected callback function as second argument"));
            return;
        }
    } else if (Buffer::HasInstance(args[0])) {
        if (args.Length() < 2 || !args[1]->IsString()) {
            isolate->ThrowException(String::New("Expected string 'format' as second argument"));
            return;
        }
        if (args.Length() < 3 || !args[2]->IsFunction()) {
            isolate->ThrowException(String::New("Expected callback function as third argument"));
            return;
        }
    } else {
        isolate->ThrowException(String::New("Expected string or buffer as first argument"));
        return;
    }


    AsyncBaton *baton = new AsyncBaton;
    baton->request.data = baton;
    baton->path = 0;
    baton->tag = NULL;
    baton->error = 0;

    if (args[0]->IsString()) {
        String::Utf8Value path(args[0]->ToString());
        baton->path = strdup(*path);
        baton->callback = Persistent<Function>::New(Local<Function>::Cast(args[1]));

    }
    else {
        baton->format = NodeStringToTagLibString(args[1]->ToString());
        baton->stream = new BufferStream(args[0]->ToObject());
        baton->callback = Persistent<Function>::New(Local<Function>::Cast(args[2]));
    }

    uv_queue_work(uv_default_loop(), &baton->request, Tag::AsyncTagReadDo, (uv_after_work_cb)Tag::AsyncTagReadAfter);

    args.GetReturnValue().SetUndefined();
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
        Local<Object> error = Object::New();
        error->Set(String::New("code"), Integer::New(baton->error));
        error->Set(String::New("message"), ErrorToString(baton->error));
        Handle<Value> argv[] = { error, Null() };
        baton->callback->Call(Context::GetCurrent()->Global(), 2, argv);
    }
    else {
        Persistent<Object> inst = Persistent<Object>::New(TagTemplate->InstanceTemplate()->NewInstance());
        baton->tag->Wrap(inst);
        Handle<Value> argv[] = { Null(), inst };
        baton->callback->Call(Context::GetCurrent()->Global(), 2, argv);
    }

    baton->callback.Dispose();
    delete baton->path;
    delete baton;
}

void Tag::AsyncSaveTag(const FunctionCallbackInfo<Value> &args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.Length() >= 1 && !args[0]->IsFunction()) {
        isolate->ThrowException(String::New("Expected callback function as first argument"));
        return;
    }

    Local<Function> callback = Local<Function>::Cast(args[0]);

    Tag *t = ObjectWrap::Unwrap<Tag>(args.This());

    AsyncBaton *baton = new AsyncBaton;
    baton->request.data = baton;
    baton->tag = t;
    baton->callback = Persistent<Function>::New(callback);
    baton->error = 1;

    uv_queue_work(uv_default_loop(), &baton->request, Tag::AsyncSaveTagDo, (uv_after_work_cb)Tag::AsyncSaveTagAfter);

    args.GetReturnValue().SetUndefined();
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
        Local<Object> error = Object::New();
        error->Set(String::New("message"), String::New("Failed to save file"));
        error->Set(String::New("path"), String::New(baton->tag->fileRef->file()->name()));
        Handle<Value> argv[] = { error };
        baton->callback->Call(Context::GetCurrent()->Global(), 1, argv);
    }
    else {
        Handle<Value> argv[] = { Null() };
        baton->callback->Call(Context::GetCurrent()->Global(), 1, argv);
    }

    baton->callback.Dispose();
    delete baton;
}

}
