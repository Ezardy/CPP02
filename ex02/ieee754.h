#ifndef EX02_IEEE754_H
# define EX02_IEEE754_H
# ifdef __linux__
#  include <ieee754.h>
# else

union ieee754_float {
float f;

struct {
#  ifdef __BIG_ENDIAN__
	unsigned int negative:1;
	unsigned int exponent:8;
	unsigned int mantissa:23;
#  else
	unsigned int mantissa:23;
	unsigned int exponent:8;
	unsigned int negative:1;
#  endif
} ieee;
};
# endif
#endif
