#ifndef NODE_TAGLIB_H
#define NODE_TAGLIB_H

#include <fileref.h>
#include <tag.h>
#include <node.h>
#include <uv.h>
#include <sys/time.h>
#include <nan.h>

#include <node_version.h>

namespace node_taglib {
class Tag;
class BufferStream;

/**
 * Note: This uses TagLib's internal file type resolvers
 * use CreateFileRef with a FileStream if the format is known
 */
int CreateFileRefPath(TagLib::FileName path, TagLib::FileRef **ref);
int CreateFileRef(TagLib::IOStream *stream, TagLib::String format, TagLib::FileRef **ref);
TagLib::File *createFile(TagLib::IOStream *stream, TagLib::String format);
v8::Handle<v8::String> ErrorToString(int error);
v8::Handle<v8::Value> TagLibStringToString( TagLib::String s );
TagLib::String NodeStringToTagLibString( v8::Local<v8::Value> s );
NAN_METHOD(AsyncReadFile);
//v8::Handle<v8::Value> AsyncReadFile(const v8::Arguments &args);
void AsyncReadFileDo(uv_work_t *req);
void AsyncReadFileAfter(uv_work_t *req);

struct AsyncBaton {
    uv_work_t request;
    v8::Persistent<v8::Function> callback;
    v8::Handle<v8::Function> localCallback;
    int error;

    TagLib::FileName path; /* only used by read/tag, not save */
    // OR
    TagLib::String format;
    BufferStream *stream; // File takes over ownership of the stream
                          // and FileRef takes over ownership of the File
                          // so don't do BufferStream deletion

    TagLib::FileRef *fileRef; /* only used by taglib.read */
    Tag *tag; /* only used by taglib.tag */
};

NAN_METHOD(AddResolvers);
//v8::Handle<v8::Value> AddResolvers(const v8::Arguments &args);

class CallbackResolver;

struct AsyncResolverBaton {
    uv_async_t request;
    const CallbackResolver *resolver;
    TagLib::FileName fileName;
    TagLib::String type;
    uv_async_t idler;
};

class CallbackResolver : public TagLib::FileRef::FileTypeResolver {
    v8::Handle<v8::Function> localResolverFunc;
    v8::Persistent<v8::Function> resolverFunc;
    const uv_thread_t created_in;

public:
    CallbackResolver(v8::Handle<v8::Function> localFunc);
    TagLib::File *createFile(TagLib::FileName fileName, bool readAudioProperties, TagLib::AudioProperties::ReadStyle audioPropertiesStyle) const;
    static void invokeResolverCb(uv_async_t *handle);
    static void stopIdling(uv_async_t *handle);
    static void invokeResolver(AsyncResolverBaton *baton);
};

}
#endif
