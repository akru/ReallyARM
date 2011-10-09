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
  strcpy(p, out);
  return out;
}

void String::strcpy(const char *strin, char *strout)
{
  do
    *strout++ = *strin;
  while (*strin++);
}

bool Math::vectEqu(const uint8_t *vect1, const uint8_t *vect2,
                   uint8_t vectSize, uint8_t err)
{
  uint8_t i = 0;
  uint8_t cerr;
  while (i++ < vectSize)
  {
    if (*vect1 > *vect2)
      cerr = *vect1++ - *vect2++;
    else
      cerr = *vect2++ - *vect1++;
    if (cerr > err)
      return false;
  }
  return true;
}
