#include "utils.h"
#include <string.h>

char * String::number(int32_t num)
{
  bool sgn = false;
  if (num < 0)
  {
    sgn = true;
    num = -num;
  }

  char str[20];
  char *p = str + 19;
  *p-- = 0;

  do
  {
    *p-- = num % 10 | '0';
    num /= 10;
  }
  while ( num );

  if ( sgn )
    *p = '-';
  else
    ++p;

  uint8_t strl = strlen(p) + 1;
  char *out = new char[strl];
  memcpy(out, p, strl);
  return out;
}
