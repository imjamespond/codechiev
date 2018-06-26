#include <stdio.h>
#include <base/Keyboard.hpp>

using codechiev::base::keyboard;
using codechiev::base::keycode;

int main ()
{
  int c = keyboard::getchar();
  printf("getchar: %d, is enter: %s \n", 
    c, c==keycode::enter?"yes":"no");

  char string [256];
  if ( keyboard::fgets (string , 100) != NULL )
  {
    printf("fgets: %s\n",string);
  }

  printf("fscanf float: ");
  float f;
  keyboard::fscanf(&f);
  printf("%f\n",f);

  printf("fscanf char *: ");
  keyboard::fscanf(string);
  printf("%s\n",string);
  return 0;
}