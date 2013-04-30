#include "metadata.h"

#include <errno.h>
#include <string.h>

#include <node_buffer.h>

#include "taglib.h"
#include "bufferstream.h"

using namespace node_taglib;
using namespace v8;
using namespace node;

namespace node_taglib {

static Persistent<FunctionTemplate> MetadataTemplate;

void InitializeMetadata(Handle<Object> target)
{
    HandleScope scope;

    MetadataTemplate = Persistent<FunctionTemplate>::New(FunctionTemplate::New());

    MetadataTemplate->SetClassName(String::NewSymbol("Metadata"));

    NODE_SET_PROTOTYPE_METHOD(MetadataTemplate, "write", MetadataAsyncWrite);
    NODE_SET_PROTOTYPE_METHOD(MetadataTemplate, "writeSync", MetadataSyncWrite);

    target->Set(String::NewSymbol("Metadata"), MetadataTemplate->GetFunction());
}

Persistent<Object> MakeMetadata(TagLib::FileRef *f)
{
    Persistent<Object> inst = Persistent<Object>::New(MetadataTemplate->InstanceTemplate()->NewInstance());

    TagLib::Tag *tag = f->tag();
    inst->Set(String::New("album"), TagLibStringToString(tag->album()));
    inst->Set(String::New("artist"), TagLibStringToString(tag->artist()));
    inst->Set(String::New("comment"), TagLibStringToString(tag->comment()));
    inst->Set(String::New("genre"), TagLibStringToString(tag->genre()));
    inst->Set(String::New("title"), TagLibStringToString(tag->title()));
    inst->Set(String::New("track"), Integer::New(tag->track()));
    inst->Set(String::New("year"), Integer::New(tag->year()));

    TagLib::AudioProperties *props = f->audioProperties();
    if (props) {
        inst->Set(String::New("length"), Integer::New(props->length()));
        inst->Set(String::New("bitrate"), Integer::New(props->bitrate()));
        inst->Set(String::New("sampleRate"), Integer::New(props->sampleRate()));
        inst->Set(String::New("channels"), Integer::New(props->channels()));
    }

    return inst;
}

bool SetFileRefMetadata(Handle<Object> inst, TagLib::FileRef *f) {
    TagLib::Tag *tag = f->tag();
    if (!tag)
        return false;

#define CHECK_AND_SET_STRING_FIELD(setter, field)\
    do { \
        Local<Value> value = inst->Get(String::New(field));\
        if (value->IsString() || value->IsNull())\
            tag->set##setter(NodeStringToTagLibString(value));\
    } while(0)

    CHECK_AND_SET_STRING_FIELD(Album, "album");
    CHECK_AND_SET_STRING_FIELD(Artist, "artist");
    CHECK_AND_SET_STRING_FIELD(Comment, "comment");
    CHECK_AND_SET_STRING_FIELD(Genre, "genre");
    CHECK_AND_SET_STRING_FIELD(Title, "title");
#undef CHECK_AND_SET_STRING_FIELD

#define CHECK_AND_SET_UINT32_FIELD(setter, field)\
    do { \
        Local<Value> value = inst->Get(String::New(field));\
        if (value->IsUint32() || value->IsNull())\
            tag->set##setter(value->Uint32Value());\
    } while(0)

    CHECK_AND_SET_UINT32_FIELD(Track, "track");
    CHECK_AND_SET_UINT32_FIELD(Year, "year");
#undef CHECK_AND_SET_UINT32_FIELD

    return true;
}

Handle<Value> MetadataSyncWrite(const Arguments &args) {
    HandleScope scope;

    Local<Object> inst = args.Holder();
    
    Local<Value> p = inst->GetHiddenValue(String::New("node_taglib::path"));
    if (p.IsEmpty() || !p->IsString()) {
        return ThrowException(String::New("Save failed: not a file")); 
    }

    String::Utf8Value path(p->ToString());

    TagLib::FileRef *f = 0;
    int error = CreateFileRefPath(*path, &f);
    if (error) {
        Local<String> fn = String::Concat(p->ToString(), Local<String>::Cast(String::New(": ", -1)));
        return ThrowException(String::Concat(fn, ErrorToString(error)));
    }

    if (!SetFileRefMetadata(inst, f)) {
        return ThrowException(String::New("Could not write metadata"));
    }

    bool success = f->save();
    if (success)
        return Undefined();
    else
        return ThrowException(String::Concat(
                  String::New("Failed to save file: "),
                  p->ToString()
              ));
}

v8::Handle<v8::Value> MetadataAsyncWrite(const v8::Arguments &args) {
    HandleScope scope;

    if (args.Length() >= 1 && !args[0]->IsFunction())
        return ThrowException(String::New("Expected callback function as first argument"));

    Local<Function> callback = Local<Function>::Cast(args[0]);

    Local<Object> inst = args.Holder();

    Local<Value> p = inst->GetHiddenValue(String::New("node_taglib::path"));
    if (p.IsEmpty() || !p->IsString()) {
        return ThrowException(String::New("Save failed: not a file")); 
    }

    AsyncBaton *baton = new AsyncBaton;
    baton->request.data = baton;
    baton->callback = Persistent<Function>::New(callback);
    baton->error = 0;
    baton->metadata = Persistent<Object>::New(inst);

    String::Utf8Value path(p->ToString());
    baton->path = strdup(*path);

    // first asynchronously create the FileRef
    uv_queue_work(uv_default_loop(), &baton->request, AsyncReadFileDo /* re-use */, (uv_after_work_cb) MetadataAsyncWriteData);
    
    return Undefined();
}

/* This runs in the v8 thread. */
void MetadataAsyncWriteData(uv_work_t *req) {
    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);
    // On error, stop processing and invoke the callback.
    if (baton->error) {
        Local<Object> error = Object::New();
        error->Set(String::New("code"), Integer::New(baton->error));
        error->Set(String::New("message"), ErrorToString(baton->error));
        Handle<Value> argv[] = { error };
        baton->callback->Call(Context::GetCurrent()->Global(), 1, argv);
        goto error;
    }
    else {
        if (!SetFileRefMetadata(baton->metadata, baton->fileRef)) {
            Local<Object> error = Object::New();
            error->Set(String::New("code"), Integer::New(-1));
            error->Set(String::New("message"), String::New("Could not write metadata"));
            Handle<Value> argv[] = { error };
            baton->callback->Call(Context::GetCurrent()->Global(), 1, argv);
            goto error;
        }

        // Now do an async write.
        // Use a new request. Relies on first member being the request.
        uv_work_t req;
        memcpy(baton, &req, uv_req_size(UV_WORK));
        baton->request.data = baton;
        uv_queue_work(uv_default_loop(), &baton->request, AsyncWriteFileDo, (uv_after_work_cb) MetadataAsyncWriteCallback);
    }

    return;

error:
    baton->callback.Dispose();
    if (baton->fileRef)
        delete baton->fileRef;
    delete baton->path;
    delete baton;
    baton = 0;
}

void AsyncWriteFileDo(uv_work_t *req) {
    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    assert(baton->fileRef);

    baton->error = !baton->fileRef->save();
}

/* This runs in the v8 thread. */
void MetadataAsyncWriteCallback(uv_work_t *req) {
    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    Handle<Value> argv[1];

    if (baton->error) {
        Local<Object> error = Object::New();
        error->Set(String::New("code"), Integer::New(-1));
        error->Set(String::New("message"), String::New("Error writing to file"));
        argv[0] = error;
    }
    else {
        argv[0] = Null();
    }

    baton->callback->Call(Context::GetCurrent()->Global(), 1, argv);

    baton->callback.Dispose();
    if (baton->fileRef)
        delete baton->fileRef;
    delete baton->path;
    delete baton;
}
}
