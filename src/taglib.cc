/*
 * Copyright (C) 2009 Nikhil Marathe ( nsm.nikhil@gmail.com )
 * This file is distributed under the MIT License. Please see
 * LICENSE for details
 */
#include <iostream>

#include <v8.h>
#include <node.h>
#include <assert.h>

#include <fileref.h>
#include <tag.h>

#include "tag.h"

using namespace v8;
using namespace node;
using namespace node_taglib;


static Persistent<FunctionTemplate> tag_constructor_template;

Handle<Value> GetFileTags(const Arguments& args)
{
    if (args.Length() < 1 || !args[0]->IsString())
        return ThrowException(String::New("Expected string 'path' as first argument"));

    String::AsciiValue path(args[0]->ToString());
    TagLib::FileRef f(*path);

    if ( f.isNull() || !f.tag() )
    {
        std::string err = "Error while reading data from ";
        String::Utf8Value val(args[0]->ToString());
        err = err + std::string(*val);
        return ThrowException( String::New( err.c_str(), err.length() ) );
    }

    TagLib::Tag *tag = f.tag();

   // Local<Object> tag_obj = tag_constructor_template->GetFunction()->NewInstance();

    return Undefined(); //tag_obj;
}

extern "C" void
init (Handle<Object> target)
{
    HandleScope scope;

    //tag_constructor_template = Persistent<FunctionTemplate>::New( FunctionTemplate::New() );

    //Local<FunctionTemplate> t = FunctionTemplate::New();
    //t->InstanceTemplate()->SetInternalFieldCount(1);

    //NODE_SET_METHOD( target, "getFileTags", GetFileTags );
    Tag::Initialize(target);
}

