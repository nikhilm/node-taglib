#ifndef NODE_TAGLIB_H
#define NODE_TAGLIB_H

#include <fileref.h>
#include <tag.h>
#include <node.h>
#include <sys/time.h>

namespace node_taglib {
class Tag;
class BufferStream;

int CreateFileRefPath(TagLib::FileName path, TagLib::FileRef **ref);
int CreateFileRefFile(TagLib::File *file, TagLib::FileRef **ref);
v8::Handle<v8::String> ErrorToString(int error);
v8::Handle<v8::Value> TagLibStringToString( TagLib::String s );
TagLib::String NodeStringToTagLibString( v8::Local<v8::Value> s );
v8::Handle<v8::Value> AddResolvers(const v8::Arguments &args);

v8::Handle<v8::Value> AsyncReadFile(const v8::Arguments &args);
void AsyncReadFileDo(uv_work_t *req);
void AsyncReadFileAfter(uv_work_t *req);

struct AsyncBaton {
    uv_work_t request;
    v8::Persistent<v8::Function> callback;
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

class CallbackResolver;

struct AsyncResolverBaton {
    uv_async_t request;
    uv_mutex_t mutex;
    const CallbackResolver *resolver;
    TagLib::FileName fileName;
    TagLib::String type;
};

class CallbackResolver : public TagLib::FileRef::FileTypeResolver {
    v8::Persistent<v8::Function> resolverFunc;
    const uv_thread_t created_in;

public:
    CallbackResolver(v8::Persistent<v8::Function> func);
    TagLib::File *createFile(TagLib::FileName fileName, bool readAudioProperties, TagLib::AudioProperties::ReadStyle audioPropertiesStyle) const;
    static void invokeResolverCb(uv_async_t *handle, int status);
    static void invokeResolver(AsyncResolverBaton *baton);
};

}
#endif
