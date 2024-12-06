#ifndef EX02_FIXED_HPP
# define EX02_FIXED_HPP

#include <ieee754.h>
#include <ostream>

class Fixed {
public:
	static Fixed		&min(Fixed &a, Fixed &b);
	static const Fixed	&min(const Fixed &a, const Fixed &b);
	static Fixed		&max(Fixed &a, Fixed &b);
	static const Fixed	&max(const Fixed &a, const Fixed &b);

	Fixed(void);
	Fixed(const int number);
	Fixed(const float number);
	Fixed(const Fixed &other);
	~Fixed(void);

	Fixed		&operator=(const Fixed &other);

	bool		operator>(const Fixed &other) const;
	bool		operator<(const Fixed &other) const;
	bool		operator>=(const Fixed &other) const;
	bool		operator<=(const Fixed &other) const;
	bool		operator==(const Fixed &other) const;
	bool		operator!=(const Fixed &other) const;

	Fixed		operator+(const Fixed &other) const;
	Fixed		operator-(const Fixed &other) const;
	Fixed		operator*(const Fixed &other) const;
	Fixed		operator/(const Fixed &other) const;

	Fixed		&operator++(void);
	Fixed		operator++(int);
	Fixed		&operator--(void);
	Fixed		operator--(int);

	int		getRawBits(void) const;
	void	setRawBits(const int raw);
	float	toFloat(void) const;
	int		toInt(void) const;
	int		getPoint(void) const;
private:
	static int	zeroes(const int v);

	static const int	_max;
	static const int	_min;
	static const ieee754_float	_maxf;
	static const ieee754_float	_minf;
	static const int	_point;

	int					_bits;
};

std::ostream	&operator<<(std::ostream &os, const Fixed &number);

#endif
