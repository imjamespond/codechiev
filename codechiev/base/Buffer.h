#ifndef FixedBufferhpp
#define FixedBufferhpp

#include <stdio.h>
#include <string>   //cpp library
#include <string.h> //c library
#include <assert.h>

namespace codechiev
{
namespace base
{

template <int BUFFER_SIZE>
struct Buffer
{
    typedef std::vector<char> buff_vec;
    FixedBuffer() : _writer(0),
                    _reader(0),
                    _buffer(BUFFER_SIZE)
    {
        ::memset(&_buffer[0], 0, _buffer.capacity());
    }

    int readable_bytes()
    {
        int rb = _writer - _reader;
        assert(rb >= 0);
        return rb;
    }

    int writable_bytes()
    {
        int wb = _buffer.capacity() - _writer;
        assert(wb >= 0);
        return wb;
    }

    int append(const char *str)
    {
        return append(str, static_cast<int>(::strlen(str)));
    }
    int append(const char *str, int len)
    {
        if (len <= 0)
            return 0;

        int wb = writable_bytes();

        //resize
        if (wb < (_buffer.capacity() >> 1) ) 
        {
            size_t buf_size = _buffer.capacity() << 1;
            _buffer.resize(buf_size);
        }

        if (wb > len)
        {
            ::memcpy(&_buffer[_writer], str, len);
            write(len);
            return len;
        }
        else if (wb > 0)
        {
            ::memcpy(&_buffer[_writer], str, wb);
            write(wb);
            return wb;
        }
        return 0;
    }

    void read(int len)
    {
        _reader += len;
    }

    void readall()
    {
        _reader = 0;
        _writer = 0;
        ::memset(&_buffer[0], 0, _buffer.capacity());
    }

    void write(int len)
    {
        _writer += len;
        assert(_writer < _buffer.capacity());
    }

    void move()
    {
        int rb = readable_bytes();
        ::memcpy(_buffer, _buffer + _reader, rb);
        _reader = 0;
        _writer = rb;
        _buffer[_writer] = 0;
    }

    // char *_buffer()
    // {
    //     return _buffer + _writer;
    // }

    // const char* str()
    // {
    //     return _buffer+_reader;
    // }

    inline int _writer() { return _writer; }
    inline int _reader() { return _reader; }

    int _writer;
    int _reader;
    const int _buffersize;
    buff_vec _buffer;
};

} // namespace base
} // namespace codechiev

#endif /* FixedBufferhpp */
