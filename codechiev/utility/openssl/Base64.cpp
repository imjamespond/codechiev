#include "Base64.h"

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <string.h>
#include <exception>

using namespace codechiev::utility;


//Encodes Base64
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <stdint.h>

std::string
Base64::Base64Encode(const unsigned char* data, size_t length)
{
    //since binary string must of contained a NULL,so we need the excact length

    //Encodes a binary safe base 64 string
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line
    BIO_write(bio, data, static_cast<int>(length));
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    BIO_set_close(bio, BIO_NOCLOSE);

    int len = static_cast<int>(bufferPtr->length);
    char* encodedData = static_cast<char *>(::malloc(len+1));
    ::memcpy(encodedData, bufferPtr->data, len);
    encodedData[len] = '\0';//need a terminator for http?
    std::string encoded(encodedData);
    ::free(encodedData);
    BIO_free_all(bio);

    return encoded; //success
}
//Decodes Base64
#include <stdio.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <stdint.h>
#include <assert.h>

int
Base64::Base64Decode(const char* b64message, unsigned_char_vec_t& vec)
{
    //Decodes a base64 encoded string
    BIO *bio, *b64;

    unsigned char buffer[16]={};
    int len(0),rtLen(0);

    len = ::strlen(b64message);
    char* b64Data = static_cast<char *>(::malloc(len+1));
    ::memcpy(b64Data, b64message, len);
    b64Data[len]='\0';

    bio = BIO_new_mem_buf(b64Data, -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
    while((len = BIO_read(bio, buffer, 16)))
    {
        std::copy(buffer, buffer+len, std::back_inserter(vec));
        ::memset(buffer, '\0', sizeof buffer);
        rtLen+=len;
    }
    ::free(b64Data);
    BIO_free_all(bio);

    return rtLen;
}