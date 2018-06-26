#include "./Keyboard.hpp"

using namespace codechiev::base;

int
keyboard::getchar()
{
  // int c;
  // puts ("Enter text. Include a dot ('.') in a sentence to exit:");
  // do {
  //   c=::getchar();
  //   putchar (c);
  // } while (c != '.');
  return ::getchar();
}

/*
  Reads characters from stream and stores them as a C string into str 
  until (num-1) characters have been read 
  or either a newline or the end-of-file is reached, 
  whichever happens first.
*/
char * 
keyboard::fgets(char * str, int num)
{
  return ::fgets (str , num , stdin);
}

/*
  ignore any whitespace
*/
template<> void 
keyboard::fscanf<float>(float * f)
{
    ::fscanf (stdin, "%f", f);
}

template<> void 
keyboard::fscanf<char>(char * str)
{
    ::fscanf (stdin, "%s", str);
}