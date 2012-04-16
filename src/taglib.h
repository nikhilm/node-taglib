#ifndef NODE_TAGLIB_H
#define NODE_TAGLIB_H

#include <fileref.h>
#include <tag.h>
#include <node.h>
#include <sys/time.h>

namespace node_taglib {
class Tag;

int CreateFileRef(TagLib::FileName path, TagLib::FileRef **ref);
v8::Handle<v8::String> ErrorToString(int error);
v8::Handle<v8::Value> TagLibStringToString( TagLib::String s );
TagLib::String NodeStringToTagLibString( v8::Local<v8::Value> s );

struct AsyncBaton {
    uv_work_t request;
    v8::Persistent<v8::Function> callback;
    int error;
    TagLib::FileName path; /* only used by read/tag, not save */
    TagLib::FileRef *fileRef; /* only used by taglib.read */
    Tag *tag; /* only used by taglib.tag */
};
}
#endif
