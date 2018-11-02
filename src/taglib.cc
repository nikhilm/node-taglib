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

Local< String > ErrorToString(int error) {
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

    return Nan::New<String>(err).ToLocalChecked();
}

void AsyncReadFile(const Nan::FunctionCallbackInfo< v8::Value >& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.Length() < 1) {
        Nan::ThrowError("Expected string or buffer as first argument");
        return;
    }

    if (args[0]->IsString()) {
        if (args.Length() < 2 || !args[1]->IsFunction()) {
            Nan::ThrowError("Expected callback function as second argument");
            return;
        }
    }
    else if (Buffer::HasInstance(args[0])) {
        if (args.Length() < 2 || !args[1]->IsString()) {
            Nan::ThrowError("Expected string 'format' as second argument");
            return;
        }
        if (args.Length() < 3 || !args[2]->IsFunction()) {
            Nan::ThrowError("Expected callback function as third argument");
            return;
        }
    }
    else {
        Nan::ThrowError("Expected string or buffer as first argument");
        return;
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
        baton->callback.Reset(Local<Function>::Cast(args[1]));

    }
    else {
        baton->format = NodeStringToTagLibString(args[1]->ToString());
        baton->stream = new BufferStream(args[0]->ToObject());
        baton->callback.Reset(Local<Function>::Cast(args[2]));
    }

    uv_queue_work(uv_default_loop(), &baton->request, AsyncReadFileDo, (uv_after_work_cb)AsyncReadFileAfter);

    args.GetReturnValue().SetUndefined();
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
    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);
    if (baton->error) {
        Local<Object> error = Nan::New<Object>();
        error->Set(Nan::New("code").ToLocalChecked(), Nan::New(baton->error));
        error->Set(Nan::New("message").ToLocalChecked(), ErrorToString(baton->error));
        Handle<Value> argv[] = { error, Nan::Null(), Nan::Null() };
        Nan::Call(Nan::New(baton->callback), Nan::GetCurrentContext()->Global(), 3, argv);
    }
    else {
        // read the data, put it in objects and delete the fileref
        TagLib::Tag *tag = baton->fileRef->tag();
        Local<Object> tagObj = Nan::New<Object>();
        if (!tag->isEmpty()) {
            tagObj->Set(Nan::New("album").ToLocalChecked(), TagLibStringToString(tag->album()));
            tagObj->Set(Nan::New("artist").ToLocalChecked(), TagLibStringToString(tag->artist()));
            tagObj->Set(Nan::New("comment").ToLocalChecked(), TagLibStringToString(tag->comment()));
            tagObj->Set(Nan::New("genre").ToLocalChecked(), TagLibStringToString(tag->genre()));
            tagObj->Set(Nan::New("title").ToLocalChecked(), TagLibStringToString(tag->title()));
            tagObj->Set(Nan::New("track").ToLocalChecked(), Nan::New(tag->track()));
            tagObj->Set(Nan::New("year").ToLocalChecked(), Nan::New(tag->year()));
        }

        TagLib::AudioProperties *props = baton->fileRef->audioProperties();
        Local<Object> propsObj = Nan::New<Object>();
        if (props) {
            propsObj->Set(Nan::New("length").ToLocalChecked(), Nan::New(props->length()));
            propsObj->Set(Nan::New("bitrate").ToLocalChecked(), Nan::New(props->bitrate()));
            propsObj->Set(Nan::New("sampleRate").ToLocalChecked(), Nan::New(props->sampleRate()));
            propsObj->Set(Nan::New("channels").ToLocalChecked(), Nan::New(props->channels()));
        }

        Handle<Value> argv[] = { Nan::Null(), tagObj, propsObj };
        Nan::Call(Nan::New(baton->callback), Nan::GetCurrentContext()->Global(), 3, argv);

        delete baton->fileRef;
        delete baton;
        baton = NULL;
    }
}

Local< Value > TagLibStringToString( TagLib::String s )
{
    if(s.isEmpty()) {
        return Nan::Null();
    }
    else {
        TagLib::ByteVector str = s.data(TagLib::String::UTF16);
        // Strip the Byte Order Mark of the input to avoid node adding a UTF-8
        // Byte Order Mark
        return Nan::New<v8::String>((uint16_t *)str.mid(2,str.size()-2).data(), s.size()).ToLocalChecked();
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

void AddResolvers(const Nan::FunctionCallbackInfo<Value> &args)
{
    for (int i = 0; i < args.Length(); i++) {
        Local<Value> arg = args[i];
        if (arg->IsFunction()) {
            Local<Function> resolver = Local<Function>::Cast(arg);
            TagLib::FileRef::addFileTypeResolver(new CallbackResolver(resolver));
        }
    }
    args.GetReturnValue().SetUndefined();
}

CallbackResolver::CallbackResolver(Local< Function > func)
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
    Nan::HandleScope scope;
    Handle<Value> argv[] = { TagLibStringToString(baton->fileName) };
    Local<Value> ret = Nan::Call(Nan::New(baton->resolver->resolverFunc), Nan::GetCurrentContext()->Global(), 1, argv).ToLocalChecked();
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
    Nan::HandleScope scope;

#ifdef TAGLIB_WITH_ASF
    Nan::Set(target, Nan::New("WITH_ASF").ToLocalChecked(), Nan::True());
#else
    Nan::Set(target, Nan::New("WITH_ASF").ToLocalChecked(), Nan::False());
#endif

#ifdef TAGLIB_WITH_MP4
    Nan::Set(target, Nan::New("WITH_MP4").ToLocalChecked(), Nan::True());
#else
    Nan::Set(target, Nan::New("WITH_MP4").ToLocalChecked(), Nan::False());
#endif

    Nan::SetMethod(target, "read", AsyncReadFile);
#ifdef ENABLE_RESOLVERS
    Nan::SetMethod(target, "addResolvers", AddResolvers);
#endif
    Tag::Init(target);
}

NODE_MODULE(taglib, init)
}
