/*
 * Copyright (C) 2009 Nikhil Marathe ( nsm.nikhil@gmail.com )
 * This file is distributed under the MIT License. Please see
 * LICENSE for details
 */
#include "taglib.h"

#include <errno.h>
#include <string.h>

#include <v8.h>
#include <node_buffer.h>

#include <tfilestream.h>
#include <asffile.h>
#include <mpegfile.h>
#include <vorbisfile.h>
#include <flacfile.h>
#include <oggflacfile.h>
#include <mpcfile.h>
#include <mp4file.h>
#include <wavpackfile.h>
#include <speexfile.h>
#include <trueaudiofile.h>
#include <aifffile.h>
#include <wavfile.h>
#include <apefile.h>
#include <modfile.h>
#include <s3mfile.h>
#include <itfile.h>
#include <xmfile.h>

#include "metadata.h"
#include "bufferstream.h"

using namespace v8;
using namespace node;
using namespace node_taglib;

namespace node_taglib {
int CreateFileRefPath(TagLib::FileName path, TagLib::FileRef **ref) {
    TagLib::FileRef *f = NULL;
    int error = 0;
    if (!TagLib::File::isReadable(path)) {
        error = EACCES;
    }
    else {
        f = new TagLib::FileRef(path);

        if (f->isNull())
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

int CreateFileRef(TagLib::IOStream *stream, TagLib::String format, TagLib::FileRef **ref) {
    TagLib::FileRef *f = NULL;
    int error = 0;

    TagLib::File *file = createFile(stream, format);
    if (file == NULL) {
        *ref = NULL;
        return EBADF;
    }

    f = new TagLib::FileRef(file);

    if (f->isNull())
    {
        error = EINVAL;
        delete f;
    }

    if (error != 0)
        *ref = NULL;
    else
        *ref = f;

    return error;
}

TagLib::File *createFile(TagLib::IOStream *stream, TagLib::String format) {
    TagLib::File *file = 0;
    format = format.upper();
    if (format == "MPEG")
        file = new TagLib::MPEG::File(stream, TagLib::ID3v2::FrameFactory::instance());
    else if (format == "OGG")
        file = new TagLib::Ogg::Vorbis::File(stream);
    else if (format == "OGG/FLAC")
        file = new TagLib::Ogg::FLAC::File(stream);
    else if (format == "FLAC")
        file = new TagLib::FLAC::File(stream, TagLib::ID3v2::FrameFactory::instance());
    else if (format == "MPC")
        file = new TagLib::MPC::File(stream);
    else if (format == "WV")
        file = new TagLib::WavPack::File(stream);
    else if (format == "SPX")
        file = new TagLib::Ogg::Speex::File(stream);
    else if (format == "TTA")
        file = new TagLib::TrueAudio::File(stream);
    else if (format == "MP4")
        file = new TagLib::MP4::File(stream);
    else if (format == "ASF")
        file = new TagLib::ASF::File(stream);
    else if (format == "AIFF")
        file = new TagLib::RIFF::AIFF::File(stream);
    else if (format == "WAV")
        file = new TagLib::RIFF::WAV::File(stream);
    else if (format == "APE")
        file = new TagLib::APE::File(stream);
    // module, nst and wow are possible but uncommon formatensions
    else if (format == "MOD")
        file = new TagLib::Mod::File(stream);
    else if (format == "S3M")
        file = new TagLib::S3M::File(stream);
    else if (format == "IT")
        file = new TagLib::IT::File(stream);
    else if (format == "XM")
        file = new TagLib::XM::File(stream);
    return file;
}

Handle<String> ErrorToString(int error) {
    HandleScope scope;
    std::string err;

    switch (error) {
        case EACCES:
            err = "File is not readable";
            break;

        case EINVAL:
            err = "Failed to extract metadata";
            break;

        case EBADF:
            err = "Unknown file format (check format string)";
            break;

        default:
            err = "Unknown error";
            break;
    }

    return scope.Close(String::New(err.c_str(), err.length()));
}

/**
 * Actually an internal function using v8 types.
 *
 * Type-checks the parameters to read(). Returns v8::True() if the arguments
 * are fine, otherwise returns an exception which can be bubbled up to JS.
 */
static Handle<Value> ProcessReadArguments(const Arguments &args, bool checkCallback) {
    if (args.Length() < 1) {
        return ThrowException(String::New("Expected path or buffer as first argument"));
    }

    // path case
    if (args[0]->IsString()) {
        if (checkCallback && (args.Length() < 2 || !args[1]->IsFunction()))
            return ThrowException(String::New("Expected callback function as second argument"));

    }
    else if (Buffer::HasInstance(args[0])) {
        if (args.Length() < 2 || !args[1]->IsString())
            return ThrowException(String::New("Expected format as second argument"));

        if (checkCallback && (args.Length() < 3 || !args[2]->IsFunction()))
            return ThrowException(String::New("Expected callback function as third argument"));

    }
    else {
        return ThrowException(String::New("Expected string or buffer as first argument"));
    }

    return True();
}

Handle<Value> ReadFile(const Arguments &args) {
    HandleScope scope;

    Handle<Value> argsCheck = ProcessReadArguments(args, false /* checkCallback */);
    if (!argsCheck->IsTrue()) {
        return scope.Close(argsCheck);
    }

    int error;
    BufferStream *buf = 0;
    TagLib::FileRef *f = 0;

    if (args[0]->IsString()) {
        String::Utf8Value path(args[0]->ToString());
        if ((error = CreateFileRefPath(*path, &f))) {
            Local<String> fn = String::Concat(args[0]->ToString(), Local<String>::Cast(String::New(": ", -1)));
            return ThrowException(String::Concat(fn, ErrorToString(error)));
        }
    }
    else {
        buf = new BufferStream(args[0]->ToObject());
        if ((error = CreateFileRef(buf, NodeStringToTagLibString(args[1]->ToString()), &f))) {
            delete buf;
            return ThrowException(ErrorToString(error));
        }
    }

    Persistent<Object> inst = MakeMetadata(f);

    if (args[0]->IsString())
        inst->SetHiddenValue(String::New("node_taglib::path"), args[0]->ToString());

    delete f;
    if (buf)
        delete buf;

    return scope.Close(inst);
}

Handle<Value> AsyncReadFile(const Arguments &args) {
    HandleScope scope;

    Handle<Value> argsCheck = ProcessReadArguments(args, true /* checkCallback */);
    if (!argsCheck->IsTrue()) {
        return scope.Close(argsCheck);
    }

    AsyncBaton *baton = new AsyncBaton;
    baton->request.data = baton;
    baton->path = 0;
    baton->format = TagLib::String::null;
    baton->stream = 0;
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

    uv_queue_work(uv_default_loop(), &baton->request, AsyncReadFileDo, (uv_after_work_cb) AsyncReadFileAfter);

    return Undefined();
}

void AsyncReadFileDo(uv_work_t *req) {
    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    TagLib::FileRef *f = 0;

    if (baton->path) {
        baton->error = node_taglib::CreateFileRefPath(baton->path, &f);
    }
    else {
        assert(baton->stream);
        baton->error = node_taglib::CreateFileRef(baton->stream, baton->format, &f);
    }

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
        // Read the data, put it in objects and delete the fileref.
        Persistent<Object> inst = MakeMetadata(baton->fileRef);
        if (baton->path)
            inst->SetHiddenValue(String::New("node_taglib::path"), TagLibStringToString(baton->path));

        delete baton->fileRef;

        Handle<Value> argv[] = { Null(), inst };
        baton->callback->Call(Context::GetCurrent()->Global(), 2, argv);
        if (baton->stream)
            delete baton->stream;

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

Handle<Value> AddResolvers(const Arguments &args)
{
    for (int i = 0; i < args.Length(); i++) {
        Local<Value> arg = args[i];
        if (arg->IsFunction()) {
            Persistent<Function> resolver = Persistent<Function>::New(Local<Function>::Cast(arg));
            TagLib::FileRef::addFileTypeResolver(new CallbackResolver(resolver));
        }
    }
    return Undefined();
}

CallbackResolver::CallbackResolver(Persistent<Function> func)
    : TagLib::FileRef::FileTypeResolver()
    , resolverFunc(func)
    // the constructor is always called in the v8 thread
#ifdef _WIN32
    , created_in(GetCurrentThreadId())
#else
    , created_in(pthread_self())
#endif
{
}

void CallbackResolver::invokeResolverCb(uv_async_t *handle, int status)
{
    AsyncResolverBaton *baton = (AsyncResolverBaton *) handle->data;
    invokeResolver(baton);
    uv_async_send((uv_async_t*) &baton->idler);
    uv_close((uv_handle_t*)&baton->request, 0);
}

void CallbackResolver::stopIdling(uv_async_t *handle, int status)
{
    uv_close((uv_handle_t*) handle, 0);
}

void CallbackResolver::invokeResolver(AsyncResolverBaton *baton)
{
    HandleScope scope;
    Handle<Value> argv[] = { TagLibStringToString(baton->fileName) };
    Local<Value> ret = baton->resolver->resolverFunc->Call(Context::GetCurrent()->Global(), 1, argv);
    if (!ret->IsString()) {
        baton->type = TagLib::String::null;
    }
    else {
        baton->type = NodeStringToTagLibString(ret->ToString()).upper();
    }
}

TagLib::File *CallbackResolver::createFile(TagLib::FileName fileName, bool readAudioProperties, TagLib::AudioProperties::ReadStyle audioPropertiesStyle) const
{
    AsyncResolverBaton baton;
    baton.request.data = (void *) &baton;
    baton.resolver = this;
    baton.fileName = fileName;

#ifdef _WIN32
    if (created_in != GetCurrentThreadId()) {
#else
    if (created_in != pthread_self()) {
#endif
        uv_loop_t *wait_loop = uv_loop_new();
        uv_async_init(wait_loop, &baton.idler, CallbackResolver::stopIdling);

        uv_async_init(uv_default_loop(), &baton.request, invokeResolverCb);
        uv_async_send(&baton.request);
        uv_run(wait_loop, UV_RUN_DEFAULT);
        uv_loop_delete(wait_loop);
    }
    else {
        invokeResolver(&baton);
    }

    TagLib::FileStream *stream = new TagLib::FileStream(fileName);

    return node_taglib::createFile(stream, baton.type);
}
}

extern "C" {

static void
init (Handle<Object> target)
{
    HandleScope scope;

#ifdef TAGLIB_WITH_ASF
    target->Set(String::NewSymbol("WITH_ASF"), True());
#else
    target->Set(String::NewSymbol("WITH_ASF"), False());
#endif

#ifdef TAGLIB_WITH_MP4
    target->Set(String::NewSymbol("WITH_MP4"), True());
#else
    target->Set(String::NewSymbol("WITH_MP4"), False());
#endif

    NODE_SET_METHOD(target, "read", AsyncReadFile);
    NODE_SET_METHOD(target, "readSync", ReadFile);
    NODE_SET_METHOD(target, "addResolvers", AddResolvers);
    InitializeMetadata(target);
}

NODE_MODULE(taglib, init)
}
