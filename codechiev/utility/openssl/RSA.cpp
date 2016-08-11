#include "RSA.h"
#include <base/Logger.hpp>

using namespace codechiev::base;
using namespace codechiev::utility;

RSA * createRSAWithFilename(const char * filename,int pub)
{
    FILE * fp = ::fopen(filename, "rb");

    if(fp == NULL)
    {
        LOG_ERROR<< "Unable to open file:"<<filename;
        return NULL;
    }
    RSA *rsa= RSA_new() ;

    if(pub)
    {
        rsa = PEM_read_RSA_PUBKEY(fp, &rsa, NULL, NULL);
    }
    else
    {
        rsa = PEM_read_RSAPrivateKey(fp, &rsa, NULL, NULL);
    }
    
    ::fclose(fp);
    
    return rsa;
}
RSA * createRSA(unsigned char * key,int pub)
{
    RSA *rsa= NULL;
    BIO *keybio ;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio==NULL)
    {
        LOG_ERROR<< "Failed to create key BIO";
        return 0;
    }
    if(pub)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    }
    if(rsa == NULL)
    {
        LOG_ERROR<< "Failed to create RSA";
    }

    return rsa;
}

int padding = RSA_PKCS1_PADDING;
using namespace codechiev::base;
RsaUtil::RsaUtil():pubRSA_(NULL),priRSA_(NULL)
{
    pubRSA_ = createRSAWithFilename(publicPem, 1);
    priRSA_ = createRSAWithFilename(privatePem, 0);
}

RsaUtil::~RsaUtil()
{
    if(pubRSA_)
        RSA_free(pubRSA_);
    if(priRSA_)
        RSA_free(priRSA_);
}

int
RsaUtil::publicPemEncrypt(const std::string& data, unsigned_char_vec& encryptedVec)
{
    unsigned char encrypted[256]={};
    int data_len = static_cast<int>(data.size());
    int length = RSA_public_encrypt(data_len, reinterpret_cast<const unsigned char*>(data.c_str()), encrypted, pubRSA_, padding);
    std::copy(encrypted, encrypted+length, std::back_inserter(encryptedVec));
    return length;
}

int
RsaUtil::privatePemDecrypt(const unsigned char* data, int dataLen, unsigned_char_vec& decryptedVec)
{
    unsigned char decrypted[256]={};
    int  length = RSA_private_decrypt(dataLen, data, decrypted, priRSA_, padding);
    std::copy(decrypted, decrypted+length, std::back_inserter(decryptedVec));
    decryptedVec.push_back('\0');
    return length;
}
