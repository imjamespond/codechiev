#ifndef Keyboard_hpp
#define Keyboard_hpp

#include <stdio.h>

namespace codechiev 
{
    namespace base 
    {
        struct keyboard 
        {
            static int getchar();
            static char * fgets(char *, int);
            template<typename T>
            static void fscanf(T *);

        };

        template<> void keyboard::fscanf<float>(float *);
        template<> void keyboard::fscanf<char >(char *);

        struct keycode
        {
            enum code
            {
                enter = 10,
                a = 97, b = 98, c = 99,
                q = 113,
                num1 = 49, num2 = 50,  num3 = 51,  num4 = 52,  num5 = 53,
            };
        };
    }
}

#endif 