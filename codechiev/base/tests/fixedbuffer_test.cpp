#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <errno.h>

#include "base/Buffer.h"
#include "base/FixedBuffer.h"

using namespace codechiev::base;

int len = 0;

int main(int argc, const char * argv[]) {

    Buffer<2,32> buffer;

    assert(buffer.writable_bytes() == 2);

    buffer.append("12345678");
    assert(buffer.readable_bytes() == 8);
    printf("append 8 bytes: %s, writable_bytes: %d\n", buffer.str().c_str(), buffer.writable_bytes());

    buffer.read(4);
    assert(buffer.readable_bytes() == 4); 
    printf("read 4 bytes: %s\n", buffer.str().c_str());

    buffer.move();
    assert(buffer._reader == 0);
    assert(buffer._writer == 4);

    buffer.append("foo");
    assert(buffer.readable_bytes() == 7);
    assert(buffer.writable_bytes() == 1);
    printf("append foo: %s\n", buffer.str().c_str());

    buffer.append("bar");
    assert(buffer.readable_bytes() == 10); 
    printf("append bar: %s\n", buffer.str().c_str());

    buffer.readall();
    assert(buffer.writable_bytes() == 16);
    len = buffer.append("12345678901234567890");
    printf("append length:%d, writable_bytes:%d, %s\n", len, buffer.writable_bytes(), buffer.str().c_str());
    len = buffer.append("12345678901234567890");
    printf("append length:%d, writable_bytes:%d, %s\n", len, buffer.writable_bytes(), buffer.str().c_str());
    len = buffer.append("12345678901234567890");
    printf("append length:%d, writable_bytes:%d, %s\n", len, buffer.writable_bytes(), buffer.str().c_str());

    printf("=========== fixed buffer tests =============\n");


    
    FixedBuffer<8> fixedbuffer;

    assert(fixedbuffer.writable_bytes() == 7);

    fixedbuffer.append("12345678");
    assert(fixedbuffer.readable_bytes() == 7);
    printf("append 8 bytes: %s\n", fixedbuffer.str());

    fixedbuffer.read(4);
    assert(fixedbuffer.readable_bytes() == 3);
    printf("read 4 bytes: %s\n", fixedbuffer.str());

    fixedbuffer.move();
    assert(fixedbuffer.reader_ == 0);
    assert(fixedbuffer.writer_ == 3);

    fixedbuffer.append("foo");
    assert(fixedbuffer.readable_bytes() == 6);
    assert(fixedbuffer.writable_bytes() == 1);
    printf("append foo: %s\n", fixedbuffer.str());

    fixedbuffer.append("bar");
    assert(fixedbuffer.readable_bytes() == 7);
    assert(fixedbuffer.writable_bytes() == 0);
    printf("append bar: %s\n", fixedbuffer.str());

    fixedbuffer.readall();
    printf("append length:%d %s\n",fixedbuffer.append("helloworld"), fixedbuffer.str());

    int n;
    fixedbuffer.readall();
    fixedbuffer.append("\r");
    for (n = 0; n < 3; n++)
    {
        fixedbuffer.append(".");
        printf(fixedbuffer.str());
        fflush(stdout); // This line flushes stdout inmediatly
        // sleep(1);
    }

    return 0;
}
