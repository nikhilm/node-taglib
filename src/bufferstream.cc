#include "bufferstream.h"

#include <node_buffer.h>

#include "taglib.h"

using namespace v8;
using namespace node;
using namespace node_taglib;

namespace node_taglib {
BufferStream::BufferStream(Handle<Object> buffer)
    : TagLib::IOStream()
    , m_data(Buffer::Data(buffer))
    , m_length(Buffer::Length(buffer))
    , m_offset(0)
{
}

BufferStream::~BufferStream()
{
}

TagLib::ByteVector BufferStream::readBlock(TagLib::ulong length) {
    long start = m_offset;
    m_offset += length;
    return TagLib::ByteVector(m_data, m_length).mid(start, length);
}

void BufferStream::writeBlock(const TagLib::ByteVector &data) {
    fprintf(stderr, "writeBlock called aborting\n");
    abort();
}

void BufferStream::insert(const TagLib::ByteVector &data, TagLib::ulong start, TagLib::ulong replace) {
    fprintf(stderr, "insert called aborting\n");
    abort();
}

void BufferStream::removeBlock(TagLib::ulong start, TagLib::ulong length) {
    fprintf(stderr, "removeBlock called aborting\n");
    abort();
}

void BufferStream::seek(long offset, TagLib::IOStream::Position p) {
    if (p == TagLib::IOStream::Beginning) {
        m_offset = offset;
    }
    else if (p == TagLib::IOStream::Current) {
        m_offset += offset;
    } 
    else if (p == TagLib::IOStream::End) {
        m_offset = length() + offset;
    }
    assert(m_offset >= 0);
}

void BufferStream::clear() {
}

long BufferStream::tell() const {
    return m_offset;
}

long BufferStream::length() {
    return m_length;
}

void BufferStream::truncate(long length) {
    fprintf(stderr, "truncate called aborting\n");
    abort();
}
}
