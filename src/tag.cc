#include "tag.h"

#include <errno.h>
#include <string.h>

#include <node_buffer.h>

#include "taglib.h"
#include "bufferstream.h"

using namespace node_taglib;
using namespace v8;
using namespace node;

namespace node_taglib {

static Persistent<FunctionTemplate> TagTemplate;

void Tag::Initialize(Handle<Object> target)
{
    NanScope();

    Local<FunctionTemplate> localTagTemplate = NanNew<FunctionTemplate>();
    NanAssignPersistent(TagTemplate, localTagTemplate);

    localTagTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    localTagTemplate->SetClassName(NanNew<String>("Tag"));

    NODE_SET_PROTOTYPE_METHOD(localTagTemplate, "save", AsyncSaveTag);
    NODE_SET_PROTOTYPE_METHOD(localTagTemplate, "saveSync", SyncSaveTag);
    NODE_SET_PROTOTYPE_METHOD(localTagTemplate, "isEmpty", IsEmpty);

    localTagTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("title"), GetTitle, SetTitle);
    localTagTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("album"), GetAlbum, SetAlbum);
    localTagTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("comment"), GetComment, SetComment);
    localTagTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("artist"), GetArtist, SetArtist);
    localTagTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("track"), GetTrack, SetTrack);
    localTagTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("year"), GetYear, SetYear);
    localTagTemplate->InstanceTemplate()->SetAccessor(NanNew<String>("genre"), GetGenre, SetGenre);

    target->Set(NanNew<String>("Tag"), localTagTemplate->GetFunction());
    NODE_SET_METHOD(target, "tag", AsyncTag);
    NODE_SET_METHOD(target, "tagSync", SyncTag);
}

Tag::Tag(TagLib::FileRef * ffileRef) : tag(ffileRef->tag()), fileRef(ffileRef) { }

Tag::~Tag() {
    if (fileRef)
        delete fileRef;
    fileRef = NULL;
    tag = NULL;
}

#define unwrapTag(info) ObjectWrap::Unwrap<Tag>(info.Holder())

NAN_GETTER(Tag::GetTitle) {
    NanScope();
    NanReturnValue(TagLibStringToString(unwrapTag(args)->tag->title()));
}

NAN_SETTER(Tag::SetTitle) {
    NanScope();
    unwrapTag(args)->tag->setTitle(NodeStringToTagLibString(value));
}

NAN_GETTER(Tag::GetArtist) {
    NanScope();
    NanReturnValue(TagLibStringToString(unwrapTag(args)->tag->artist()));
}

NAN_SETTER(Tag::SetArtist) {
    NanScope();
    unwrapTag(args)->tag->setArtist(NodeStringToTagLibString(value));
}

NAN_GETTER(Tag::GetAlbum) {
    NanScope();
    NanReturnValue(TagLibStringToString(unwrapTag(args)->tag->album()));
}

NAN_SETTER(Tag::SetAlbum) {
    NanScope();
    unwrapTag(args)->tag->setAlbum(NodeStringToTagLibString(value));
}

NAN_GETTER(Tag::GetComment) {
    NanScope();
    NanReturnValue(TagLibStringToString(unwrapTag(args)->tag->comment()));
}

NAN_SETTER(Tag::SetComment) {
    NanScope();
    unwrapTag(args)->tag->setComment(NodeStringToTagLibString(value));
}

NAN_GETTER(Tag::GetTrack) {
    NanScope();
    NanReturnValue(NanNew<Integer>(unwrapTag(args)->tag->track()));
}

NAN_SETTER(Tag::SetTrack) {
    NanScope();
    unwrapTag(args)->tag->setTrack(value->IntegerValue());
}

NAN_GETTER(Tag::GetYear) {
    NanScope();
    NanReturnValue(NanNew<Integer>(unwrapTag(args)->tag->year()));
}

NAN_SETTER(Tag::SetYear) {
    NanScope();
    unwrapTag(args)->tag->setYear(value->IntegerValue());
}

NAN_GETTER(Tag::GetGenre) {
    NanScope();
    NanReturnValue(TagLibStringToString(unwrapTag(args)->tag->genre()));
}

NAN_SETTER(Tag::SetGenre) {
    NanScope();
    unwrapTag(args)->tag->setGenre(NodeStringToTagLibString(value));
}

NAN_METHOD(Tag::IsEmpty) {
    NanScope();
    Tag *t = ObjectWrap::Unwrap<Tag>(args.This());
    NanReturnValue(NanNew<Boolean>(t->tag->isEmpty()));
}

NAN_METHOD(Tag::SyncSaveTag) {
    NanScope();
    Tag *t = ObjectWrap::Unwrap<Tag>(args.This());
    assert(t->fileRef);
    bool success = t->fileRef->save();
    if (success)
        NanReturnUndefined();
    else
        return NanThrowError(String::Concat(
                NanNew<String>("Failed to save file: "),
                NanNew<String>(t->fileRef->file()->name())
            ));
}

NAN_METHOD(Tag::SyncTag) {
    NanScope();

    TagLib::FileRef *f = 0;
    int error = 0;

    if (args.Length() >= 1 && args[0]->IsString()) {
        String::Utf8Value path(args[0]->ToString());
        if ((error = CreateFileRefPath(*path, &f))) {
            Local<String> fn = String::Concat(args[0]->ToString(), Local<String>::Cast(NanNew<String>(": ", -1)));
            return NanThrowError(String::Concat(fn, ErrorToString(error)));
        }
    }
    else if (args.Length() >= 1 && Buffer::HasInstance(args[0])) {
        if (args.Length() < 2 || !args[1]->IsString())
            return NanThrowError("Expected string 'format' as second argument");

        if ((error = CreateFileRef(new BufferStream(args[0]->ToObject()), NodeStringToTagLibString(args[1]->ToString()), &f))) {
            return NanThrowError(ErrorToString(error));
        }
    }
    else {
        return NanThrowError("Expected string or buffer as first argument");
    }

    Tag * tag = new Tag(f);
    Handle<Object> inst = NanNew(TagTemplate)->InstanceTemplate()->NewInstance();
    tag->Wrap(inst);

    NanReturnValue(inst);
}

NAN_METHOD(Tag::AsyncTag) {
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
    baton->tag = NULL;
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

    uv_queue_work(uv_default_loop(), &baton->request, Tag::AsyncTagReadDo, (uv_after_work_cb)Tag::AsyncTagReadAfter);

    NanReturnUndefined();
}

void Tag::AsyncTagReadDo(uv_work_t *req) {
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
        baton->tag = new Tag(f);
    }
}

void Tag::AsyncTagReadAfter(uv_work_t *req) {
    NanScope();

    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    if (baton->error) {
        Local<Object> error = NanNew<Object>();
        error->Set(NanNew<String>("code"), NanNew<Integer>(baton->error));
        error->Set(NanNew<String>("message"), ErrorToString(baton->error));
        Handle<Value> argv[] = { error, NanNull() };
        NanNew(baton->callback)->Call(NanGetCurrentContext()->Global(), 2, argv);
    }
    else {
        Persistent<Object> inst;
        Handle<Object> localInst = NanNew(TagTemplate)->InstanceTemplate()->NewInstance();
        NanAssignPersistent(inst, localInst);
        baton->tag->Wrap(localInst);
        Handle<Value> argv[] = { NanNull(), localInst };
        NanNew(baton->callback)->Call(NanGetCurrentContext()->Global(), 2, argv);
    }

    NanDisposePersistent(baton->callback);
    delete baton->path;
    delete baton;
}

NAN_METHOD(Tag::AsyncSaveTag) {
    NanScope();

    if (args.Length() >= 1 && !args[0]->IsFunction())
        return NanThrowError("Expected callback function as first argument");

    Local<Function> callback = Local<Function>::Cast(args[0]);

    Tag *t = ObjectWrap::Unwrap<Tag>(args.This());

    AsyncBaton *baton = new AsyncBaton;
    baton->request.data = baton;
    baton->tag = t;
    NanAssignPersistent(baton->callback, callback);
    baton->error = 1;

    uv_queue_work(uv_default_loop(), &baton->request, Tag::AsyncSaveTagDo, (uv_after_work_cb)Tag::AsyncSaveTagAfter);

    NanReturnUndefined();
}

void Tag::AsyncSaveTagDo(uv_work_t *req) {
    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    assert(baton->tag->fileRef);
    baton->error = !baton->tag->fileRef->save();
}

void Tag::AsyncSaveTagAfter(uv_work_t *req) {
    NanScope();

    AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

    if (baton->error) {
        Local<Object> error = NanNew<Object>();
        error->Set(NanNew<String>("message"), NanNew<String>("Failed to save file"));
        error->Set(NanNew<String>("path"), NanNew<String>(baton->tag->fileRef->file()->name()));
        Handle<Value> argv[] = { error };
        NanNew(baton->callback)->Call(NanGetCurrentContext()->Global(), 1, argv);
    }
    else {
        Handle<Value> argv[] = { NanNull() };
        NanNew(baton->callback)->Call(NanGetCurrentContext()->Global(), 1, argv);
    }

    NanDisposePersistent(baton->callback);
    delete baton;
}

}
