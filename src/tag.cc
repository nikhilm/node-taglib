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

static suseconds_t now()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_usec;
}

static Persistent<FunctionTemplate> TagTemplate;

void Tag::Initialize(Handle<Object> target)
{
    HandleScope scope;

    TagTemplate = Persistent<FunctionTemplate>::New(FunctionTemplate::New());

    TagTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    TagTemplate->SetClassName(String::NewSymbol("Tag"));

    NODE_SET_PROTOTYPE_METHOD(TagTemplate, "save", AsyncSaveTag);
    NODE_SET_PROTOTYPE_METHOD(TagTemplate, "saveSync", SyncSaveTag);
    NODE_SET_PROTOTYPE_METHOD(TagTemplate, "isEmpty", IsEmpty);

    TagTemplate->InstanceTemplate()->SetAccessor(String::New("title"), GetTitle, SetTitle);
    TagTemplate->InstanceTemplate()->SetAccessor(String::New("album"), GetAlbum, SetAlbum);
    TagTemplate->InstanceTemplate()->SetAccessor(String::New("comment"), GetComment, SetComment);
    TagTemplate->InstanceTemplate()->SetAccessor(String::New("artist"), GetArtist, SetArtist);
    TagTemplate->InstanceTemplate()->SetAccessor(String::New("track"), GetTrack, SetTrack);
    TagTemplate->InstanceTemplate()->SetAccessor(String::New("year"), GetYear, SetYear);
    TagTemplate->InstanceTemplate()->SetAccessor(String::New("genre"), GetGenre, SetGenre);

    target->Set(String::NewSymbol("Tag"), TagTemplate->GetFunction());
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

inline Tag * unwrapTag(const AccessorInfo& info) {
  return ObjectWrap::Unwrap<Tag>(info.Holder());
}

Handle<Value> Tag::GetTitle(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(TagLibStringToString(unwrapTag(info)->tag->title()));
}

void Tag::SetTitle(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  unwrapTag(info)->tag->setTitle(NodeStringToTagLibString(value));
}

Handle<Value> Tag::GetArtist(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(TagLibStringToString(unwrapTag(info)->tag->artist()));
}

void Tag::SetArtist(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  unwrapTag(info)->tag->setArtist(NodeStringToTagLibString(value));
}

Handle<Value> Tag::GetAlbum(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(TagLibStringToString(unwrapTag(info)->tag->album()));
}

void Tag::SetAlbum(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  unwrapTag(info)->tag->setAlbum(NodeStringToTagLibString(value));
}

Handle<Value> Tag::GetComment(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(TagLibStringToString(unwrapTag(info)->tag->comment()));
}

void Tag::SetComment(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  unwrapTag(info)->tag->setComment(NodeStringToTagLibString(value));
}

Handle<Value> Tag::GetTrack(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(Integer::New(unwrapTag(info)->tag->track()));
}

void Tag::SetTrack(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  unwrapTag(info)->tag->setTrack(value->IntegerValue());
}

Handle<Value> Tag::GetYear(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(Integer::New(unwrapTag(info)->tag->year()));
}

void Tag::SetYear(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  unwrapTag(info)->tag->setYear(value->IntegerValue());
}

Handle<Value> Tag::GetGenre(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(TagLibStringToString(unwrapTag(info)->tag->genre()));
}

void Tag::SetGenre(Local<String> property, Local<Value> value, const AccessorInfo& info) {
  HandleScope scope;
  unwrapTag(info)->tag->setGenre(NodeStringToTagLibString(value));
}

Handle<Value> Tag::IsEmpty(const Arguments &args) {
  HandleScope scope;
  Tag *t = ObjectWrap::Unwrap<Tag>(args.This());
  return Boolean::New(t->tag->isEmpty());
}

Handle<Value> Tag::SyncSaveTag(const Arguments &args) {
  HandleScope scope;
  Tag *t = ObjectWrap::Unwrap<Tag>(args.This());
  assert(t->fileRef);
  bool success = t->fileRef->save();
  if (success)
      return Undefined();
  else
      return ThrowException(String::Concat(
                String::New("Failed to save file: "),
                String::New(t->fileRef->file()->name())
            ));
}

Handle<Value> Tag::SyncTag(const Arguments &args) {
    HandleScope scope;

    TagLib::FileRef *f = 0;
    int error = 0;

    if (args.Length() >= 1 && args[0]->IsString()) {
        String::Utf8Value path(args[0]->ToString());
        if (error = CreateFileRefPath(*path, &f)) {
            Local<String> fn = String::Concat(args[0]->ToString(), Local<String>::Cast(String::New(": ", -1)));
            return ThrowException(String::Concat(fn, ErrorToString(error)));
        }
    }
    else if (args.Length() >= 1 && Buffer::HasInstance(args[0])) {
        if (args.Length() < 2 || !args[1]->IsString())
            return ThrowException(String::New("Expected string 'format' as second argument"));

        if (error = CreateFileRef(new BufferStream(args[0]->ToObject()), NodeStringToTagLibString(args[1]->ToString()), &f)) {
            return ThrowException(ErrorToString(error));
        }
    }
    else {
        return ThrowException(String::New("Expected string or buffer as first argument"));
    }

    Tag * tag = new Tag(f);
    Handle<Object> inst = TagTemplate->InstanceTemplate()->NewInstance();
    tag->Wrap(inst);

    return scope.Close(inst);
}

v8::Handle<v8::Value> Tag::AsyncTag(const v8::Arguments &args) {
    HandleScope scope;

    if (args.Length() < 1) {
        return ThrowException(String::New("Expected string or buffer as first argument"));
    }

    if (args[0]->IsString()) {
        if (args.Length() < 2 || !args[1]->IsFunction())
            return ThrowException(String::New("Expected callback function as second argument"));

    }
    else if (Buffer::HasInstance(args[0])) {
        if (args.Length() < 2 || !args[1]->IsString())
            return ThrowException(String::New("Expected string 'format' as second argument"));
        if (args.Length() < 3 || !args[2]->IsFunction())
            return ThrowException(String::New("Expected callback function as third argument"));
    }
    else {
        return ThrowException(String::New("Expected string or buffer as first argument"));
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

    uv_queue_work(uv_default_loop(), &baton->request, Tag::AsyncTagRead, Tag::AsyncTagReadAfter);

    return Undefined();
}

void Tag::AsyncTagRead(uv_work_t *req) {
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
    HandleScope scope;

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

v8::Handle<v8::Value> Tag::AsyncSaveTag(const v8::Arguments &args) {
    HandleScope scope;

    if (args.Length() >= 1 && !args[0]->IsFunction())
        return ThrowException(String::New("Expected callback function as first argument"));

    Local<Function> callback = Local<Function>::Cast(args[0]);

    Tag *t = ObjectWrap::Unwrap<Tag>(args.This());

    AsyncBaton *baton = new AsyncBaton;
    baton->request.data = baton;
    baton->tag = t;
    baton->callback = Persistent<Function>::New(callback);
    baton->error = 1;

    uv_queue_work(uv_default_loop(), &baton->request, Tag::AsyncSaveTagDo, Tag::AsyncSaveTagAfter);

    return Undefined();
}

void Tag::AsyncSaveTagDo(uv_work_t *req) {
    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    assert(baton->tag->fileRef);
    baton->error = !baton->tag->fileRef->save();
}

void Tag::AsyncSaveTagAfter(uv_work_t *req) {
    HandleScope scope;

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
