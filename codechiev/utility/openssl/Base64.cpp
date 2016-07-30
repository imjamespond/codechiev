#include "Base64.h"

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <string.h>
#include <exception>

using namespace codechiev::utility;

std::string
Base64::encode(const std::string& message)
{
    BIO *bio, *b64;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_fp(stdout, BIO_NOCLOSE);
    BIO_push(b64, bio);
    BIO_write(b64, message.c_str(), ::strlen(message.c_str()));
    BIO_flush(b64);

    BIO_free_all(b64);

    return "";
}

std::string
Base64::decode(const std::string& message)
{
    BIO *bio, *b64, *bio_out;
    char inbuf[512];
    int inlen;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_fp(stdin, BIO_NOCLOSE);
    bio_out = BIO_new_fp(stdout, BIO_NOCLOSE);
    BIO_push(b64, bio);
    while((inlen = BIO_read(b64, inbuf, 512)) > 0)
        BIO_write(bio_out, inbuf, inlen);

    BIO_flush(bio_out);
    BIO_free_all(b64);

    return "";
}

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

int calcDecodeLength(const char* b64input) { //Calculates the length of a decoded string
    size_t len = strlen(b64input),
    padding = 0;

    if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
        padding = 2;
    else if (b64input[len-1] == '=') //last char is =
        padding = 1;

    return static_cast<int>((len*3)/4 - padding);
}

int
Base64::Base64Decode(const char* b64message, unsigned_char_vec_t& vec, size_t* length)
{
    //Decodes a base64 encoded string
    BIO *bio, *b64;

    unsigned char buffer[16]={};
    int inlen;

    bio = BIO_new_mem_buf(b64message, -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
    while((inlen = BIO_read(bio, buffer, 16)))
    {
        std::copy(buffer, buffer+inlen, std::back_inserter(vec));
        ::memset(buffer, '\0', sizeof buffer);
        *length+=inlen;
    }

    BIO_free_all(bio);

    return (0); //success
}
