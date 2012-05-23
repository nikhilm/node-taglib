#ifndef NODE_TAGLIB_BUFFERSTREAM_H
#define NODE_TAGLIB_BUFFERSTREAM_H

#include <node.h>

#include <taglib.h>
#include <tstring.h>
#include <tiostream.h>

namespace node_taglib {
class BufferStream : public TagLib::IOStream {
public:
    BufferStream(v8::Handle<v8::Object> buffer);

    ~BufferStream();

    TagLib::FileName name() const { return TagLib::String::null.toCString(); }

    TagLib::ByteVector readBlock(TagLib::ulong length);
    void writeBlock(const TagLib::ByteVector &data);
    void insert(const TagLib::ByteVector &data, TagLib::ulong start=0, TagLib::ulong replace=0);
    void removeBlock(TagLib::ulong start=0, TagLib::ulong length=0);
    bool readOnly() const { return true; }
    bool isOpen() const { return true; }
    void seek(long offset, TagLib::IOStream::Position p=TagLib::IOStream::Beginning);
    void clear();
    long tell() const;
    long length();
    void truncate(long length);

private:
    char *m_data;
    long m_length;
    long m_offset;
};
}
#endif
