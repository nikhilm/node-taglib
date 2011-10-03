#include "audioproperties.h"

using namespace node_taglib;
using namespace v8;
using namespace node;

static Persistent<FunctionTemplate> pft;

void AudioProperties::Initialize(Handle<Object> target)
{
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);
    pft = Persistent<FunctionTemplate>::New(t);
    pft->InstanceTemplate()->SetInternalFieldCount(1);
    pft->SetClassName(String::NewSymbol("AudioProperties"));

    pft->InstanceTemplate()->SetAccessor(String::New("length"), GetLength);
    pft->InstanceTemplate()->SetAccessor(String::New("bitrate"), GetBitrate);
    pft->InstanceTemplate()->SetAccessor(String::New("sampleRate"), GetSampleRate);
    pft->InstanceTemplate()->SetAccessor(String::New("channels"), GetChannels);
    target->Set(String::NewSymbol("AudioProperties"), pft->GetFunction());
}

AudioProperties::AudioProperties(TagLib::FileRef * ffileRef) : properties(ffileRef->audioProperties()), fileRef(ffileRef) { }

inline AudioProperties * unwrapAudioProperties(const AccessorInfo& info) {
  return ObjectWrap::Unwrap<AudioProperties>(info.Holder());
}

Handle<Value> AudioProperties::GetLength(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(Integer::New(unwrapAudioProperties(info)->properties->length()));
}

Handle<Value> AudioProperties::GetBitrate(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(Integer::New(unwrapAudioProperties(info)->properties->bitrate()));
}

Handle<Value> AudioProperties::GetSampleRate(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(Integer::New(unwrapAudioProperties(info)->properties->sampleRate()));
}

Handle<Value> AudioProperties::GetChannels(Local<String> property, const AccessorInfo& info) {
  HandleScope scope;
  return scope.Close(Integer::New(unwrapAudioProperties(info)->properties->channels()));
}

Handle<Value> AudioProperties::New(const Arguments &args) {
    HandleScope scope;

    if (args.Length() < 1 || !args[0]->IsString())
        return ThrowException(String::New("Expected string 'path' as first argument"));

    String::Utf8Value path(args[0]->ToString());

    TagLib::FileRef * f = new TagLib::FileRef(*path);
    if ( f->isNull() || !f->audioProperties() )
    {
        std::string err = "Error while reading data from ";
        err = err + std::string(*path);
        return ThrowException( String::New( err.c_str(), err.length() ) );
    }

    AudioProperties * properties = new AudioProperties(f);
    properties->Wrap(args.This());

    return args.This();
}
