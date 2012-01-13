/*
 * Copyright (C) 2009 Nikhil Marathe ( nsm.nikhil@gmail.com )
 * This file is distributed under the MIT License. Please see
 * LICENSE for details
 */
#include <v8.h>

#include "audioproperties.h"
#include "tag.h"

using namespace v8;
using namespace node;
using namespace node_taglib;


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
