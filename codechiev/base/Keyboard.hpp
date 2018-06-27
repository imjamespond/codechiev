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

        struct keycode
        {
            enum code
            {
                enter = 10,
            };
        };

        template<> void keyboard::fscanf<float>(float *);
        template<> void keyboard::fscanf<char >(char *);
    }
}

#endif 