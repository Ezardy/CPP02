#include <iostream>
#include <limits>
#include <ieee754.h>

#include "Fixed.hpp"

Fixed::Fixed(void) : _bits(0) {
	std::cout << "Default constructor called\n";
}

Fixed::Fixed(const Fixed &other) {
	std::cout << "Copy constructor called\n";
	*this = other;
}

Fixed::Fixed(const int number) : _bits(number << _point) {
	if (number & ~0 << (sizeof(int) * 8 - _point))
		throw std::overflow_error("The integer is too big for the Fixed type");
	else
		std::cout << "Int constructor called\n";
}

Fixed::Fixed(const float number) : _bits(0) {
	const union ieee754_float	*fp
		= reinterpret_cast<const union ieee754_float *>(&number);
	short	e = fp->ieee.exponent
		- std::numeric_limits<float>::max_exponent + 1;

	if (number != 0 && e > - _point - 1) {
		if (fp->ieee.exponent == ~'\0')
			throw std::invalid_argument("Presented float is NAN or INF");
		else if (e > static_cast<int>(sizeof(float)) * 8 - _point - 1
				|| (e == static_cast<int>(sizeof(float)) * 8 - _point - 1
					&& !fp->ieee.negative))
			throw std::overflow_error(
				"Presented float is out of the Fixed type's range"
			);
		else {
			std::cout << "Float constructor called\n";
			_bits = fp->ieee.mantissa | 1 << 23;
			bool	must_reverse = fp->ieee.negative
				&& e < static_cast<int>(sizeof(float)) * 8 - _point - 1;
			e -= std::numeric_limits<float>::digits - _point - 1;
			if (e > 0)
				_bits <<= e;
			else
				_bits >>= -e;
			if (must_reverse)
				_bits *= -1;
		}
	}
}

Fixed::~Fixed(void) {
	std::cout << "Destructor called\n";
}

Fixed	&Fixed::operator=(const Fixed &other) {
	if (this != &other) {
		std::cout << "Copy assignment operator called\n";
		_bits = other.getRawBits();
	}
	return *this;
}

int	Fixed::getRawBits(void) const {
	std::cout << "getRawBits member function called\n";
	return _bits;
}

void	Fixed::setRawBits(const int raw) {
	std::cout << "setRawBits member function called\n";
	_bits = raw;
}

float	Fixed::toFloat(void) const {
	union ieee754_float	fp;

	if (_bits == 0)
		fp.f = 0;
	else if (_bits == std::numeric_limits<int>::min()) {
		fp.ieee.negative = 1;
		fp.ieee.exponent = std::numeric_limits<float>::max_exponent - 1
			+ sizeof(int) - _point - 1;
		fp.ieee.mantissa = 0;
	} else {
		int		m = _bits;
		short	e;

		if (_bits < 0) {
			m *= -1;
			fp.ieee.negative = 1;
		}
		for (e = 0; !(m & 1 << std::numeric_limits<int>::digits);
			m <<= 1, e += 1);
		m = m << 1
			>> (sizeof(float) * 8 - std::numeric_limits<float>::digits + 1);
		e = std::numeric_limits<int>::digits - _point - e
			+ std::numeric_limits<float>::max_exponent - 1;
		fp.ieee.exponent = e;
		fp.ieee.mantissa = m;
	}
	return fp.f;
}

int	Fixed::toInt(void) const {
	return _bits >> _point;
}

const int	Fixed::_point = 8;

std::ostream	&operator<<(std::ostream &os, const Fixed &number) {
	os << number.toFloat();
	return os;
}
