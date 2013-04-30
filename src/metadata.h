#ifndef NODE_TAGLIB_METADATA_H
#define NODE_TAGLIB_METADATA_H

#include <fileref.h>
#include <node.h>
#include <sys/time.h>

namespace node_taglib {
void InitializeMetadata(v8::Handle<v8::Object> target);

v8::Persistent<v8::Object> MakeMetadata(TagLib::FileRef *f);
bool SetFileRefMetadata(v8::Handle<v8::Object> inst, TagLib::FileRef *f);

v8::Handle<v8::Value> MetadataSyncWrite(const v8::Arguments &args);

v8::Handle<v8::Value> MetadataAsyncWrite(const v8::Arguments &args);
void MetadataAsyncWriteData(uv_work_t *req);
void AsyncWriteFileDo(uv_work_t *req);
void MetadataAsyncWriteCallback(uv_work_t *req);
}
#endif
