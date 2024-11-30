#ifndef EX01_FIXED_HPP
# define EX01_FIXED_HPP

#include <ostream>

class Fixed {
public:
	static const int	max;
	static const int	min;

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
	static const int	_point;
	int					_bits;
};

std::ostream	&operator<<(std::ostream &os, const Fixed &number);

#endif
