#ifndef BASE64_H
#define BASE64_H

#include <string>
#include <vector>

namespace codechiev
{
    namespace utility
    {
        class Base64
        {
        public:
            typedef std::vector<unsigned char> unsigned_char_vec_t;

            Base64();
            ~Base64();

            std::string encode(const std::string&);
            std::string decode(const std::string&);
            static std::string Base64Encode(const unsigned char* , size_t );
            static int Base64Decode(const char* , unsigned_char_vec_t& );

        protected:
        private:
        };

        typedef Base64::unsigned_char_vec_t unsigned_char_vec;
    }
}

#endif // BASE64_H
