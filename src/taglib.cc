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

#include "audioproperties.h"
#include "tag.h"
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

int CreateFileRef(TagLib::IOStream *stream, TagLib::String format, TagLib::FileRef **ref) {
    TagLib::FileRef *f = NULL;
    int error = 0;

    TagLib::File *file = createFile(stream, format);
    if (file == NULL) {
        *ref = NULL;
        return EBADF;
    }

    f = new TagLib::FileRef(file);

    if (f->isNull() || !f->tag())
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
    NanEscapableScope();
    std::string err;

    switch (error) {
        case EACCES:
            err = "File is not readable";
            break;

        case EINVAL:
            err = "Failed to extract tags";
            break;

        case EBADF:
            err = "Unknown file format (check format string)";
            break;

        default:
            err = "Unknown error";
            break;
    }

    return NanEscapeScope(NanNew<String>(err.c_str(), err.length()));
}

NAN_METHOD(AsyncReadFile) {
    NanScope();


    if (args.Length() < 1) {
        return NanThrowError("Expected string or buffer as first argument");
    }

    if (args[0]->IsString()) {
        if (args.Length() < 2 || !args[1]->IsFunction())
            return NanThrowError("Expected callback function as second argument");

    }
    else if (Buffer::HasInstance(args[0])) {
        if (args.Length() < 2 || !args[1]->IsString())
            return NanThrowError("Expected string 'format' as second argument");
        if (args.Length() < 3 || !args[2]->IsFunction())
            return NanThrowError("Expected callback function as third argument");
    }
    else {
        return NanThrowError("Expected string or buffer as first argument");
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
        NanAssignPersistent(baton->callback, Local<Function>::Cast(args[1]));

    }
    else {
        baton->format = NodeStringToTagLibString(args[1]->ToString());
        baton->stream = new BufferStream(args[0]->ToObject());
        NanAssignPersistent(baton->callback, Local<Function>::Cast(args[2]));
    }

    uv_queue_work(uv_default_loop(), &baton->request, AsyncReadFileDo, (uv_after_work_cb)AsyncReadFileAfter);

    NanReturnUndefined();
}

void AsyncReadFileDo(uv_work_t *req) {
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
        baton->fileRef = f;
    }
}

void AsyncReadFileAfter(uv_work_t *req) {
    NanScope();
    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);
    if (baton->error) {
        Local<Object> error = NanNew<Object>();
        error->Set(NanNew<String>("code"), NanNew<Integer>(baton->error));
        error->Set(NanNew<String>("message"), ErrorToString(baton->error));
        Handle<Value> argv[] = { error, NanNull(), NanNull() };
        NanNew(baton->callback)->Call(NanGetCurrentContext()->Global(), 3, argv);
    }
    else {
        // read the data, put it in objects and delete the fileref
        TagLib::Tag *tag = baton->fileRef->tag();
        Local<Object> tagObj = NanNew<Object>();
        if (!tag->isEmpty()) {
            tagObj->Set(NanNew<String>("album"), TagLibStringToString(tag->album()));
            tagObj->Set(NanNew<String>("artist"), TagLibStringToString(tag->artist()));
            tagObj->Set(NanNew<String>("comment"), TagLibStringToString(tag->comment()));
            tagObj->Set(NanNew<String>("genre"), TagLibStringToString(tag->genre()));
            tagObj->Set(NanNew<String>("title"), TagLibStringToString(tag->title()));
            tagObj->Set(NanNew<String>("track"), NanNew<Integer>(tag->track()));
            tagObj->Set(NanNew<String>("year"), NanNew<Integer>(tag->year()));
        }

        TagLib::AudioProperties *props = baton->fileRef->audioProperties();
        Local<Object> propsObj = NanNew<Object>();
        if (props) {
            propsObj->Set(NanNew<String>("length"), NanNew<Integer>(props->length()));
            propsObj->Set(NanNew<String>("bitrate"), NanNew<Integer>(props->bitrate()));
            propsObj->Set(NanNew<String>("sampleRate"), NanNew<Integer>(props->sampleRate()));
            propsObj->Set(NanNew<String>("channels"), NanNew<Integer>(props->channels()));
        }

        Handle<Value> argv[] = { NanNull(), tagObj, propsObj };
        NanNew(baton->callback)->Call(NanGetCurrentContext()->Global(), 3, argv);

        delete baton->fileRef;
        delete baton;
        baton = NULL;
    }
}

Handle<Value> TagLibStringToString( TagLib::String s )
{
  NanEscapableScope();
    if(s.isEmpty()) {
        return NanNull();
    }
    else {
        TagLib::ByteVector str = s.data(TagLib::String::UTF16);
        // Strip the Byte Order Mark of the input to avoid node adding a UTF-8
        // Byte Order Mark
	Handle<String> v8string = NanNew<String>((uint16_t *)str.mid(2,str.size()-2).data(), s.size());
        return NanEscapeScope(v8string);
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

NAN_METHOD(AddResolvers)
{
  NanScope();
    for (int i = 0; i < args.Length(); i++) {
        Local<Value> arg = args[i];
        if (arg->IsFunction()) {
            TagLib::FileRef::addFileTypeResolver(new CallbackResolver(Local<Function>::Cast(arg)));
        }
    }
    NanReturnUndefined();
}

CallbackResolver::CallbackResolver(Handle<Function> func)
    : TagLib::FileRef::FileTypeResolver()
    // the constructor is always called in the v8 thread
#ifdef _WIN32
    , created_in(GetCurrentThreadId())
#else
    , created_in(pthread_self())
#endif
{
  NanAssignPersistent(resolverFunc, func);
}

void CallbackResolver::invokeResolverCb(uv_async_t *handle)
{
    AsyncResolverBaton *baton = (AsyncResolverBaton *) handle->data;
    invokeResolver(baton);
    uv_async_send((uv_async_t*) &baton->idler);
    uv_close((uv_handle_t*)&baton->request, 0);
}

void CallbackResolver::stopIdling(uv_async_t *handle)
{
    uv_close((uv_handle_t*) handle, 0);
}

void CallbackResolver::invokeResolver(AsyncResolverBaton *baton)
{
  NanScope();
    Handle<Value> argv[] = { TagLibStringToString(baton->fileName) };
    Local<Value> ret = NanNew(baton->resolver->resolverFunc)->Call(NanGetCurrentContext()->Global(), 1, argv);
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
  NanScope();

#ifdef TAGLIB_WITH_ASF
    target->Set(NanNew<String>("WITH_ASF"), NanTrue());
#else
    target->Set(NanNew<String>("WITH_ASF"), NanFalse());
#endif

#ifdef TAGLIB_WITH_MP4
    target->Set(NanNew<String>("WITH_MP4"), NanTrue());
#else
    target->Set(NanNew<String>("WITH_MP4"), NanFalse());
#endif

    NODE_SET_METHOD(target, "read", AsyncReadFile);
#ifdef ENABLE_RESOLVERS
    NODE_SET_METHOD(target, "addResolvers", AddResolvers);
#endif
    Tag::Initialize(target);
}

NODE_MODULE(taglib, init)
}
