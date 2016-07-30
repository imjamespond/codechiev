#ifndef RSA_H
#define RSA_H

#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include <assert.h>

namespace codechiev{
    namespace utility{

        static const char publicPem[] = "public.pem";
        static const char privatePem[] = "private.pem";

        class RsaUtil
        {
        public:
            RsaUtil();
            ~RsaUtil();

            int publicPemEncrypt(const std::string&, Base64::unsignedchar_vec& );
            int privatePemDecrypt(const unsigned char*, int, Base64::unsignedchar_vec& );
        private:
            RSA* pubRSA_;
            RSA* priRSA_;
        };

    }
}

#endif // RSA_H
