#ifndef NODE_TAGLIB_AUDIOPROPERTIES_H
#define NODE_TAGLIB_AUDIOPROPERTIES_H

#include <fileref.h>
#include <audioproperties.h>
#include <node.h>

namespace node_taglib {
class AudioProperties : public node::ObjectWrap {
    TagLib::AudioProperties * properties;
    TagLib::FileRef * fileRef;

  public:
    static void Initialize(v8::Handle<v8::Object> target);
    AudioProperties(TagLib::FileRef * fileRef);
    ~AudioProperties();

    static v8::Handle<v8::Value> New(const v8::Arguments &args);

    static v8::Handle<v8::Value> GetLength(v8::Local<v8::String> property, const v8::AccessorInfo& info);
    static v8::Handle<v8::Value> GetBitrate(v8::Local<v8::String> property, const v8::AccessorInfo& info);
    static v8::Handle<v8::Value> GetSampleRate(v8::Local<v8::String> property, const v8::AccessorInfo& info);
    static v8::Handle<v8::Value> GetChannels(v8::Local<v8::String> property, const v8::AccessorInfo& info);
};
}
#endif
