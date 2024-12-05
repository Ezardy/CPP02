#include <iostream>
#include <limits>
#include <cmath>

#include "Fixed.hpp"

Fixed::Fixed(void) : _bits(0) {
	std::cout << "Default constructor called\n";
}

Fixed::Fixed(const Fixed &other) {
	std::cout << "Copy constructor called\n";
	*this = other;
}

Fixed::Fixed(const int number) : _bits(number << _point) {
	if (number > _max || number < _min)
		throw std::overflow_error("The integer is too big for the Fixed type");
	else
		std::cout << "Int constructor called\n";
}

Fixed::Fixed(const float number) : _bits(0) {
	union ieee754_float	fp;

	fp.f = number;
	short	e = fp.ieee.exponent
		- std::numeric_limits<float>::max_exponent + 1;
	if (fp.f != fp.f || fp.f == std::numeric_limits<float>::infinity()
		|| fp.f == -std::numeric_limits<float>::infinity())
		throw std::invalid_argument("Presented float is NAN or INF");
	else if (number > _maxf.f || number < _minf.f)
		throw std::overflow_error(
			"Presented float is out of the Fixed type's range"
		);
	else {
		std::cout << "Float constructor called\n";
		if (number != 0 && e > - _point - 1) {
			fp.ieee.exponent += _point;
			fp.f = roundf(fp.f);
			_bits = fp.ieee.mantissa
				| 1 << (std::numeric_limits<float>::digits - 1);
			bool	must_reverse = fp.ieee.negative
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
	return _bits;
}

void	Fixed::setRawBits(const int raw) {
	_bits = raw;
}

float	Fixed::toFloat(void) const {
	float	fp = toInt();
	float	fract = static_cast<float>(_bits - toInt() * (1 << _point))
		/ (1 << _point);

	return fp + fract;
}

int	Fixed::toInt(void) const {
	return _bits / (1 << _point);
}

const int	Fixed::_point = 8;

std::ostream	&operator<<(std::ostream &os, const Fixed &number) {
	if (number.getRawBits() << (sizeof(int) * 8 - number.getPoint()) == 0)
		os << number.toInt();
	else
		os << number.toFloat();
	return os;
}

int	Fixed::getPoint(void) const {
	return _point;
}

const int			Fixed::_max = ~0u >> (_point + 1);

const int			Fixed::_min = ~0u << (sizeof(int) * 8 - _point - 1);

const ieee754_float	Fixed::_maxf = {
	.ieee = {
		.mantissa = ~0u
			>> (sizeof(float) * 8 - std::numeric_limits<float>::digits + 1),
		.exponent = std::numeric_limits<float>::max_exponent - 1
			+ sizeof(int) * 8 - _point - 2,
		.negative = 0
	}
};

const ieee754_float	Fixed::_minf = {
	.ieee = {
		.mantissa = 0,
		.exponent = std::numeric_limits<float>::max_exponent - 1
			+ sizeof(int) * 8 - _point - 1,
		.negative = 1
	}
};
