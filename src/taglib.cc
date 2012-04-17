/*
 * Copyright (C) 2009 Nikhil Marathe ( nsm.nikhil@gmail.com )
 * This file is distributed under the MIT License. Please see
 * LICENSE for details
 */
#include "taglib.h"

#include <errno.h>
#include <string.h>

#include <v8.h>

#include "audioproperties.h"
#include "tag.h"

using namespace v8;
using namespace node;
using namespace node_taglib;

namespace node_taglib {
int CreateFileRef(TagLib::FileName path, TagLib::FileRef **ref) {
    TagLib::FileRef *f = NULL;
    int error = 0;
    if (!TagLib::File::isReadable(path)) {
        error = EACCES;
    }
    else {
        f = new TagLib::FileRef(path);

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

v8::Handle<v8::Value> AsyncReadFile(const v8::Arguments &args) {
    HandleScope scope;

    if (args.Length() < 1 || !args[0]->IsString())
        return ThrowException(String::New("Expected string 'path' as first argument"));

    String::Utf8Value path(args[0]->ToString());

    if (args.Length() < 2 || !args[1]->IsFunction())
        return ThrowException(String::New("Expected callback function as second argument"));

    Local<Function> callback = Local<Function>::Cast(args[1]);

    AsyncBaton *baton = new AsyncBaton;
    baton->request.data = baton;
    baton->path = strdup(*path);
    baton->callback = Persistent<Function>::New(callback);
    baton->error = 0;

    uv_queue_work(uv_default_loop(), &baton->request, AsyncReadFileDo, AsyncReadFileAfter);

    return Undefined();
}

void AsyncReadFileDo(uv_work_t *req) {
    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    TagLib::FileRef *f;
    int error;

    baton->error = node_taglib::CreateFileRef(baton->path, &f);

    if (baton->error == 0) {
        baton->fileRef = f;
    }
}

void AsyncReadFileAfter(uv_work_t *req) {
    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);
    if (baton->error) {
        Local<Object> error = Object::New();
        error->Set(String::New("code"), Integer::New(baton->error));
        error->Set(String::New("message"), ErrorToString(baton->error));
        Handle<Value> argv[] = { error, Null(), Null() };
        baton->callback->Call(Context::GetCurrent()->Global(), 3, argv);
    }
    else {
        // read the data, put it in objects and delete the fileref
        TagLib::Tag *tag = baton->fileRef->tag();
        Local<Object> tagObj = Object::New();
        if (!tag->isEmpty()) {
            tagObj->Set(String::New("album"), TagLibStringToString(tag->album()));
            tagObj->Set(String::New("artist"), TagLibStringToString(tag->artist()));
            tagObj->Set(String::New("comment"), TagLibStringToString(tag->comment()));
            tagObj->Set(String::New("genre"), TagLibStringToString(tag->genre()));
            tagObj->Set(String::New("title"), TagLibStringToString(tag->title()));
            tagObj->Set(String::New("track"), Integer::New(tag->track()));
            tagObj->Set(String::New("year"), Integer::New(tag->year()));
        }

        TagLib::AudioProperties *props = baton->fileRef->audioProperties();
        Local<Object> propsObj = Object::New();
        if (props) {
            propsObj->Set(String::New("length"), Integer::New(props->length()));
            propsObj->Set(String::New("bitrate"), Integer::New(props->bitrate()));
            propsObj->Set(String::New("sampleRate"), Integer::New(props->sampleRate()));
            propsObj->Set(String::New("channels"), Integer::New(props->channels()));
        }

        Handle<Value> argv[] = { Null(), tagObj, propsObj };
        baton->callback->Call(Context::GetCurrent()->Global(), 3, argv);

        delete baton->fileRef;
        delete baton;
        baton = NULL;
    }
}

Handle<Value> TagLibStringToString( TagLib::String s )
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

TagLib::String NodeStringToTagLibString( Local<Value> s )
{
  if(s->IsNull()) {
    return TagLib::String::null;
  }
  else {
    String::Utf8Value str(s->ToString());
    return TagLib::String(*str, TagLib::String::UTF8);
  }
}

}

extern "C" {

static void
init (Handle<Object> target)
{
    HandleScope scope;

#ifdef TAGLIB_WITH_ASF
    target->Set(String::NewSymbol("WITH_ASF"), v8::True());
#else
    target->Set(String::NewSymbol("WITH_ASF"), v8::False());
#endif

#ifdef TAGLIB_WITH_MP4
    target->Set(String::NewSymbol("WITH_MP4"), v8::True());
#else
    target->Set(String::NewSymbol("WITH_MP4"), v8::False());
#endif

    NODE_SET_METHOD(target, "read", AsyncReadFile);
    Tag::Initialize(target);
}

NODE_MODULE(taglib, init)
}
