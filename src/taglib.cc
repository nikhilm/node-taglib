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

    AudioProperties::Initialize(target);
    Tag::Initialize(target);
}

NODE_MODULE(taglib, init)
}
