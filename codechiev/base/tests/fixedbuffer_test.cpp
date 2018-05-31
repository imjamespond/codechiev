#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <errno.h>

#include "base/FixedBuffer.h"

using namespace codechiev::base;


int main(int argc, const char * argv[]) {
    
    FixedBuffer<8> buffer;

    assert(buffer.writable_bytes() == 7);

    buffer.append("12345678");
    assert(buffer.readable_bytes() == 7);
    printf("append: %s\n", buffer.str());

    buffer.read(4);
    assert(buffer.readable_bytes() == 3);
    printf("read: %s\n", buffer.str());

    buffer.move();
    assert(buffer.reader_ == 0);
    assert(buffer.writer_ == 3);
    printf("move: %s\n", buffer.str());

    buffer.append("foo");
    assert(buffer.readable_bytes() == 6);
    assert(buffer.writable_bytes() == 1);
    printf("append:%s\n", buffer.str());

    buffer.append("bar");
    assert(buffer.readable_bytes() == 7);
    assert(buffer.writable_bytes() == 0);
    printf("append:%s\n", buffer.str());

    buffer.readall();
    printf("append:%d %s\n",buffer.append("helloworld"), buffer.str());

    return 0;
}
