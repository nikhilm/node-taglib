#include <errno.h>

#include "tag.h"

using namespace node_taglib;
using namespace v8;
using namespace node;

namespace node_taglib {

static Persistent<FunctionTemplate> TagTemplate;

void Tag::Initialize(Handle<Object> target)
{
    HandleScope scope;

    TagTemplate = Persistent<FunctionTemplate>::New(FunctionTemplate::New());

    TagTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    TagTemplate->SetClassName(String::NewSymbol("Tag"));

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
Tag::~Tag() { delete fileRef; }

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
  return Boolean::New(t->fileRef->save());
}

Handle<Value> Tag::SyncTag(const Arguments &args) {
    HandleScope scope;

    if (args.Length() < 1 || !args[0]->IsString())
        return ThrowException(String::New("Expected string 'path' as first argument"));

    String::Utf8Value path(args[0]->ToString());

    TagLib::FileRef *f;
    int error;
    if ((error = node_taglib::CreateFileRef(*path, &f)) != 0) {
        Local<String> fn = String::Concat(args[0]->ToString(), Local<String>::Cast(String::New(": ", -1)));
        return ThrowException(String::Concat(fn, ErrorToString(error)));
    }

    Tag * tag = new Tag(f);
    Handle<Object> inst = TagTemplate->InstanceTemplate()->NewInstance();
    tag->Wrap(inst);

    return scope.Close(inst);
}

Handle<Value> Tag::TagLibStringToString( TagLib::String s )
{
  if(s.isEmpty()) {
    return Null();
  }
  else {
    TagLib::ByteVector str = s.data(TagLib::String::UTF16);
    // Strip the Byte Order Mark of the input to avoid node adding a UTF-8
    // Byte Order Mark
    return String::New((uint16_t *)str.mid(2,str.size()-2).data(), s.size());
  }
}

TagLib::String Tag::NodeStringToTagLibString( Local<Value> s )
{
  if(s->IsNull()) {
    return TagLib::String::null;
  }
  else {
    String::Utf8Value str(s->ToString());
    return TagLib::String(*str, TagLib::String::UTF8);
  }
}

int CreateFileRef(TagLib::FileName path, TagLib::FileRef **ref) {
    TagLib::FileRef *f = NULL;
    int error = 0;
    if (!TagLib::File::isReadable(path)) {
        error = EACCES;
    }
    else {
        f = new TagLib::FileRef(path, false /* skip reading audioProperties */);

        if ( f->isNull() || !f->tag() )
        {
            error = EINVAL;
            delete f;
        }
    }

    if (error != 0)
        *ref = NULL;
    else
        *ref = f;

    return error;
}

Handle<String> ErrorToString(int error) {
    HandleScope scope;
    std::string err;

    switch (error) {
        case EACCES:
            err = "File is not readable";
            break;

        case EINVAL:
            err = "Failed to extract tags";
            break;

        default:
            err = "Unknown error";
            break;
    }

    return scope.Close(String::New(err.c_str(), err.length()));
}

v8::Handle<v8::Value> Tag::AsyncTag(const v8::Arguments &args) {
    HandleScope scope;

    if (args.Length() < 1 || !args[0]->IsString())
        return ThrowException(String::New("Expected string 'path' as first argument"));

    String::Utf8Value path(args[0]->ToString());

    if (args.Length() < 2 || !args[1]->IsFunction())
        return ThrowException(String::New("Expected callback function as second argument"));

    Local<Function> callback = Local<Function>::Cast(args[1]);

    AsyncTagBaton *baton = new AsyncTagBaton;
    baton->request.data = baton;
    baton->path = strdup(*path);
    baton->tag = NULL;
    baton->callback = Persistent<Function>::New(callback);
    baton->error = 0;

    uv_queue_work(Loop(), &baton->request, Tag::AsyncTagRead, Tag::AsyncTagReadAfter);

    return Undefined();
}

void Tag::AsyncTagRead(uv_work_t *req) {
    AsyncTagBaton *baton = static_cast<AsyncTagBaton*>(req->data);

    TagLib::FileRef *f;
    int error;

    baton->error = node_taglib::CreateFileRef(baton->path, &f);

    if (baton->error == 0) {
        baton->tag = new Tag(f);
    }
}

void Tag::AsyncTagReadAfter(uv_work_t *req) {
    HandleScope scope;

    AsyncTagBaton *baton = static_cast<AsyncTagBaton*>(req->data);

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
}
