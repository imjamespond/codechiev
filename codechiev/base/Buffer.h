#ifndef FixedBufferhpp
#define FixedBufferhpp

#include <stdio.h>
#include <string>   //cpp library
#include <string.h> //c library
#include <assert.h>
#include <vector>

namespace codechiev
{
namespace base
{

template <int INITIAL_SIZE, int MAXIMUM_SIZE = 1024 * 1024>
struct Buffer
{
    typedef std::vector<char> buff_vec;
    Buffer() : _writer(0),
               _reader(0),
               _buffer(INITIAL_SIZE)
    {
        ::memset(&_buffer[0], 0, _buffer.capacity());
    }

    std::string str()
    {
        std::string str(&_buffer[_reader], readable_bytes());
        return str;
    }

    const char *buf()
    {
        return &_buffer[_reader];
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
    int append(const char *str, int len, int reserved_size = INITIAL_SIZE)
    {
        if (len <= 0)
            return 0;

        int writable = writable_bytes();

        if ((writable >> 1) < reserved_size)
        {
            resize(writable, reserved_size);
        }

        if (writable >= len)
        {
            ::memcpy(&_buffer[_writer], str, len);
            // ::printf("_buffer[0]: %c\n", _buffer[0]);
            write(len);
            return len;
        }

        return -1;
    }

    void resize(int &writable, int len)
    {
        size_t size = _buffer.capacity();
        for (;;)
        {
            size = size << 1;

            if (size > MAXIMUM_SIZE)
            {
                writable = writable_bytes();
                break;
            }

            _buffer.resize(size); //compare to reserve, resize allocates space (if needed) and initialize it
            writable = writable_bytes();

            if (writable >= len)
            {
                // printf("writable_bytes: %d, %d\n", writable, len);
                break;
            }
        }


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
        assert(_writer <= _buffer.capacity());
    }

    void move()
    {
        int rb = readable_bytes();
        ::memcpy(&_buffer[0], &_buffer[_reader], rb);
        _reader = 0;
        _writer = rb;
        _buffer[_writer] = 0;
    }

    inline int writer() { return _writer; }
    inline int reader() { return _reader; }

    int _writer;
    int _reader;
    buff_vec _buffer;
};

} // namespace base
} // namespace codechiev

#endif /* FixedBufferhpp */
