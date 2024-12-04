#ifndef EX01_FIXED_HPP
# define EX01_FIXED_HPP

#include <ostream>
#include <ieee754.h>

class Fixed {
public:

	Fixed(void);
	Fixed(const int number);
	Fixed(const float number);
	Fixed(const Fixed &other);
	~Fixed(void);

	Fixed	&operator=(const Fixed &other);

	int		getRawBits(void) const;
	void	setRawBits(const int raw);
	float	toFloat(void) const;
	int		toInt(void) const;
	int		getPoint(void) const;
private:
	static const int			_max;
	static const int			_min;
	static const ieee754_float	_maxf;
	static const ieee754_float	_minf;

	static const int	_point;
	int					_bits;
};

std::ostream	&operator<<(std::ostream &os, const Fixed &number);

#endif
