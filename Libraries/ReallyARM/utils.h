#include <stdint.h>

namespace String {
char * number(int32_t num);
void strcpy(const char *strin, char *strout);
}

namespace Math {
bool vectEqu(const uint8_t *vect1, const uint8_t *vect2,
             uint8_t vectSize, uint8_t err);
}
