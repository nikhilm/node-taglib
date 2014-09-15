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

static Local<FunctionTemplate> localTagTemplate;
static Persistent<FunctionTemplate> TagTemplate;

void Tag::Initialize(Handle<Object> target)
{
  NanScope();
//    HandleScope scope;

  localTagTemplate = NanNew<FunctionTemplate>();
  NanAssignPersistent(TagTemplate, localTagTemplate);
//    TagTemplate = Persistent<FunctionTemplate>::New(FunctionTemplate::New());

    localTagTemplate->InstanceTemplate()->SetInternalFieldCount(1);
//    localTagTemplate->SetClassName(String::NewSymbol("Tag"));

    NODE_SET_PROTOTYPE_METHOD(localTagTemplate, "save", AsyncSaveTag);
    NODE_SET_PROTOTYPE_METHOD(localTagTemplate, "saveSync", SyncSaveTag);
    NODE_SET_PROTOTYPE_METHOD(localTagTemplate, "isEmpty", IsEmpty);

    localTagTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("title"), GetTitle, SetTitle);
    localTagTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("album"), GetAlbum, SetAlbum);
    localTagTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("comment"), GetComment, SetComment);
    localTagTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("artist"), GetArtist, SetArtist);
    localTagTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("track"), GetTrack, SetTrack);
    localTagTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("year"), GetYear, SetYear);
    localTagTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("genre"), GetGenre, SetGenre);

    target->Set(NanNew<String>("Tag"), localTagTemplate->GetFunction());
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

//inline Tag * unwrapTag(const AccessorInfo& info) {
//  return ObjectWrap::Unwrap<Tag>(info.Holder());
//}

#define unwrapTag(info) ObjectWrap::Unwrap<Tag>(info.Holder())

NAN_GETTER(Tag::GetTitle) {
    NanScope();
    NanReturnValue(TagLibStringToString(unwrapTag(args)->tag->title()));
//Handle<Value> Tag::GetTitle(Local<String> property, const AccessorInfo& info) {
//  HandleScope scope;
//  return scope.Close(TagLibStringToString(unwrapTag(info)->tag->title()));
}

NAN_SETTER(Tag::SetTitle) {
    NanScope();
//void Tag::SetTitle(Local<String> property, Local<Value> value, const AccessorInfo& info) {
//  HandleScope scope;
  unwrapTag(args)->tag->setTitle(NodeStringToTagLibString(value));
}

NAN_GETTER(Tag::GetArtist) {
    NanScope();
    NanReturnValue(TagLibStringToString(unwrapTag(args)->tag->artist()));
//Handle<Value> Tag::GetArtist(Local<String> property, const AccessorInfo& info) {
//  HandleScope scope;
//  return scope.Close(TagLibStringToString(unwrapTag(info)->tag->artist()));
}

NAN_SETTER(Tag::SetArtist) {
    NanScope();
//void Tag::SetArtist(Local<String> property, Local<Value> value, const AccessorInfo& info) {
//  HandleScope scope;
  unwrapTag(args)->tag->setArtist(NodeStringToTagLibString(value));
}

NAN_GETTER(Tag::GetAlbum) {
    NanScope();
    NanReturnValue(TagLibStringToString(unwrapTag(args)->tag->album()));
//Handle<Value> Tag::GetAlbum(Local<String> property, const AccessorInfo& info) {
//  HandleScope scope;
//  return scope.Close(TagLibStringToString(unwrapTag(info)->tag->album()));
}

NAN_SETTER(Tag::SetAlbum) {
    NanScope();
//void Tag::SetAlbum(Local<String> property, Local<Value> value, const AccessorInfo& info) {
//  HandleScope scope;
  unwrapTag(args)->tag->setAlbum(NodeStringToTagLibString(value));
}

NAN_GETTER(Tag::GetComment) {
    NanScope();
    NanReturnValue(TagLibStringToString(unwrapTag(args)->tag->comment()));
//Handle<Value> Tag::GetComment(Local<String> property, const AccessorInfo& info) {
//  HandleScope scope;
//  return scope.Close(TagLibStringToString(unwrapTag(info)->tag->comment()));
}

NAN_SETTER(Tag::SetComment) {
    NanScope();
//void Tag::SetComment(Local<String> property, Local<Value> value, const AccessorInfo& info) {
//  HandleScope scope;
  unwrapTag(args)->tag->setComment(NodeStringToTagLibString(value));
}

NAN_GETTER(Tag::GetTrack) {
    NanScope();
    NanReturnValue(NanNew<Integer>(unwrapTag(args)->tag->track()));
//Handle<Value> Tag::GetTrack(Local<String> property, const AccessorInfo& info) {
//  HandleScope scope;
//  return scope.Close(Integer::New(unwrapTag(info)->tag->track()));
}

NAN_SETTER(Tag::SetTrack) {
    NanScope();
//void Tag::SetTrack(Local<String> property, Local<Value> value, const AccessorInfo& info) {
//  HandleScope scope;
  unwrapTag(args)->tag->setTrack(value->IntegerValue());
}

NAN_GETTER(Tag::GetYear) {
    NanScope();
    NanReturnValue(NanNew<Integer>(unwrapTag(args)->tag->year()));
//Handle<Value> Tag::GetYear(Local<String> property, const AccessorInfo& info) {
//  HandleScope scope;
//  return scope.Close(Integer::New(unwrapTag(info)->tag->year()));
}

NAN_SETTER(Tag::SetYear) {
    NanScope();
//void Tag::SetYear(Local<String> property, Local<Value> value, const AccessorInfo& info) {
//  HandleScope scope;
  unwrapTag(args)->tag->setYear(value->IntegerValue());
}

NAN_GETTER(Tag::GetGenre) {
    NanScope();
    NanReturnValue(TagLibStringToString(unwrapTag(args)->tag->genre()));
//Handle<Value> Tag::GetGenre(Local<String> property, const AccessorInfo& info) {
//  HandleScope scope;
//  return scope.Close(TagLibStringToString(unwrapTag(info)->tag->genre()));
}

NAN_SETTER(Tag::SetGenre) {
    NanScope();
//void Tag::SetGenre(Local<String> property, Local<Value> value, const AccessorInfo& info) {
//  HandleScope scope;
  unwrapTag(args)->tag->setGenre(NodeStringToTagLibString(value));
}

NAN_METHOD(Tag::IsEmpty) {
    NanScope();
//Handle<Value> Tag::IsEmpty(const Arguments &args) {
//  HandleScope scope;
  Tag *t = ObjectWrap::Unwrap<Tag>(args.This());
  NanReturnValue(NanNew<Boolean>(t->tag->isEmpty()));
}

NAN_METHOD(Tag::SyncSaveTag) {
    NanScope();
//Handle<Value> Tag::SyncSaveTag(const Arguments &args) {
//  HandleScope scope;
  Tag *t = ObjectWrap::Unwrap<Tag>(args.This());
  assert(t->fileRef);
  bool success = t->fileRef->save();
  if (success)
      NanReturnUndefined();
  else
      NanThrowError(String::Concat(
                NanNew<String>("Failed to save file: "),
                NanNew<String>(t->fileRef->file()->name())
            ));
}

NAN_METHOD(Tag::SyncTag) {
    NanScope();
//Handle<Value> Tag::SyncTag(const Arguments &args) {
//    HandleScope scope;

    TagLib::FileRef *f = 0;
    int error = 0;

    if (args.Length() >= 1 && args[0]->IsString()) {
        String::Utf8Value path(args[0]->ToString());
        if ((error = CreateFileRefPath(*path, &f))) {
            Local<String> fn = String::Concat(args[0]->ToString(), Local<String>::Cast(NanNew<String>(": ", -1)));
            NanThrowError(String::Concat(fn, ErrorToString(error)));
        }
    }
    else if (args.Length() >= 1 && Buffer::HasInstance(args[0])) {
        if (args.Length() < 2 || !args[1]->IsString())
            NanThrowError("Expected string 'format' as second argument");

        if ((error = CreateFileRef(new BufferStream(args[0]->ToObject()), NodeStringToTagLibString(args[1]->ToString()), &f))) {
            NanThrowError(ErrorToString(error));
        }
    }
    else {
        NanThrowError("Expected string or buffer as first argument");
    }

    Tag * tag = new Tag(f);
    Handle<Object> inst = localTagTemplate->InstanceTemplate()->NewInstance();
    tag->Wrap(inst);

    NanReturnValue(inst);
//    return scope.Close(inst);
}

NAN_METHOD(Tag::AsyncTag) {
    NanScope();
//v8::Handle<v8::Value> Tag::AsyncTag(const v8::Arguments &args) {
//    HandleScope scope;

    if (args.Length() < 1) {
        NanThrowError("Expected string or buffer as first argument");
    }

    if (args[0]->IsString()) {
        if (args.Length() < 2 || !args[1]->IsFunction())
            NanThrowError("Expected callback function as second argument");

    }
    else if (Buffer::HasInstance(args[0])) {
        if (args.Length() < 2 || !args[1]->IsString())
            NanThrowError("Expected string 'format' as second argument");
        if (args.Length() < 3 || !args[2]->IsFunction())
            NanThrowError("Expected callback function as third argument");
    }
    else {
        NanThrowError("Expected string or buffer as first argument");
    }


    AsyncBaton *baton = new AsyncBaton;
    baton->request.data = baton;
    baton->path = 0;
    baton->tag = NULL;
    baton->error = 0;

    if (args[0]->IsString()) {
        String::Utf8Value path(args[0]->ToString());
        baton->path = strdup(*path);
	baton->localCallback = Local<Function>::Cast(args[1]);
        NanAssignPersistent(baton->callback, baton->localCallback);

    }
    else {
        baton->format = NodeStringToTagLibString(args[1]->ToString());
        baton->stream = new BufferStream(args[0]->ToObject());
	baton->localCallback = Local<Function>::Cast(args[2]);
        NanAssignPersistent(baton->callback, baton->localCallback);
    }

    uv_queue_work(uv_default_loop(), &baton->request, Tag::AsyncTagReadDo, (uv_after_work_cb)Tag::AsyncTagReadAfter);

    NanReturnUndefined();
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
    NanScope();
//    HandleScope scope;

    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    if (baton->error) {
        Local<Object> error = NanNew<Object>();
        error->Set(NanNew<String>("code"), NanNew<Integer>(baton->error));
        error->Set(NanNew<String>("message"), ErrorToString(baton->error));
        Handle<Value> argv[] = { error, NanNull() };
        baton->localCallback->Call(NanGetCurrentContext()->Global(), 2, argv);
    }
    else {
        Persistent<Object> inst;
	Handle<Object> localInst = localTagTemplate->InstanceTemplate()->NewInstance();
	NanAssignPersistent(inst, localInst);
        baton->tag->Wrap(localInst);
        Handle<Value> argv[] = { NanNull(), localInst };
        baton->localCallback->Call(NanGetCurrentContext()->Global(), 2, argv);
    }

    NanDisposePersistent(baton->callback);
    delete baton->path;
    delete baton;
}

NAN_METHOD(AsyncSaveTag) {
//v8::Handle<v8::Value> Tag::AsyncSaveTag(const v8::Arguments &args) {
    NanScope();
//    HandleScope scope;

    if (args.Length() >= 1 && !args[0]->IsFunction())
        NanThrowError("Expected callback function as first argument");

    Local<Function> callback = Local<Function>::Cast(args[0]);

    Tag *t = ObjectWrap::Unwrap<Tag>(args.This());

    AsyncBaton *baton = new AsyncBaton;
    baton->request.data = baton;
    baton->tag = t;
    NanAssignPersistent(baton->callback, callback);
    baton->error = 1;

    uv_queue_work(uv_default_loop(), &baton->request, Tag::AsyncSaveTagDo, (uv_after_work_cb)Tag::AsyncSaveTagAfter);

    NanReturnUndefined();
}

void Tag::AsyncSaveTagDo(uv_work_t *req) {
    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    assert(baton->tag->fileRef);
    baton->error = !baton->tag->fileRef->save();
}

void Tag::AsyncSaveTagAfter(uv_work_t *req) {
    NanScope();

    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    if (baton->error) {
        Local<Object> error = NanNew<Object>();
        error->Set(NanNew<String>("message"), NanNew<String>("Failed to save file"));
        error->Set(NanNew<String>("path"), NanNew<String>(baton->tag->fileRef->file()->name()));
        Handle<Value> argv[] = { error };
        baton->localCallback->Call(NanGetCurrentContext()->Global(), 1, argv);
    }
    else {
        Handle<Value> argv[] = { NanNull() };
        baton->localCallback->Call(NanGetCurrentContext()->Global(), 1, argv);
    }

    NanDisposePersistent(baton->callback);
    delete baton;
}

}
