#include <iostream>
#include <limits>
#include <cmath>

#include "Fixed.hpp"

Fixed::Fixed(void) : _bits(0) {
}

Fixed::Fixed(const Fixed &other) {
	*this = other;
}

Fixed::Fixed(const int number) : _bits(number << _point) {
	if (number > _max || number < _min)
		throw std::overflow_error("The integer is too big for the Fixed type");
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
}

Fixed	&Fixed::operator=(const Fixed &other) {
	if (this != &other) {
		_bits = other.getRawBits();
	}
	return *this;
}

bool	Fixed::operator>(const Fixed &other) const {
	return _bits > other._bits;
}

bool	Fixed::operator<(const Fixed &other) const {
	return _bits < other._bits;
}

bool	Fixed::operator>=(const Fixed &other) const {
	return _bits >= other._bits;
}

bool	Fixed::operator<=(const Fixed &other) const {
	return _bits <= other._bits;
}

bool	Fixed::operator==(const Fixed &other) const {
	return _bits == other._bits;
}

bool	Fixed::operator!=(const Fixed &other) const {
	return _bits != other._bits;
}

Fixed	Fixed::operator+(const Fixed &other) const {
	Fixed	r;

	if (_bits == 0 || other._bits == 0 ||
		(_bits > 0 && other._bits < 0) || (_bits < 0 && other._bits > 0)
		|| (_bits > 0 && std::numeric_limits<int>::max() - _bits >= other._bits)
		|| (_bits < 0 && std::numeric_limits<int>::min() - _bits <= other._bits))
		r._bits = _bits + other._bits;
	else
		throw std::overflow_error("Add operator overflowed");
	return r;
}

Fixed	Fixed::operator-(const Fixed &other) const {
	Fixed	r;

	if (_bits == 0 || other._bits == 0
		|| (_bits > 0 && other._bits > 0) || (_bits < 0 && other._bits < 0)
		|| (_bits > 0 && std::numeric_limits<int>::max() + other._bits >= _bits)
		|| (_bits < 0 && std::numeric_limits<int>::min() + other._bits <= _bits))
		r._bits = _bits - other._bits;
	else
		throw std::overflow_error("Subtract operator overflowed");
	return r;
}

Fixed	Fixed::operator*(const Fixed &other) const {
	Fixed	r;
	const int			i1 = _bits / (1 << _point);
	const int			i2 = other._bits / (1 << _point);

	try {
		if (!i1 || !i2
			|| (i1 > 0 && i2 > 0 && i1 <= std::numeric_limits<int>::max() / i2)
			|| (i1 < 0 && i2 < 0 && i1 >= std::numeric_limits<int>::max() / i2)
			|| (i1 > 0 && i2 < 0 && i2 >= std::numeric_limits<int>::min() / i1)
			|| (i1 < 0 && i2 > 0 && i1 >= std::numeric_limits<int>::min() / i2)) {
			Fixed	ii(i1 * i2);
			Fixed	_if;
			Fixed	fi;
			Fixed	ff;
			int		f1 = _bits - i1 * (1 << _point);
			int		f2 = other._bits - i2 * (1 << _point);

			_if._bits = i1 * f2;
			fi._bits = i2 * f1;
			ff._bits = f1 * f2;
			ff._bits >>= _point;
			r = ii + _if + fi + ff;
		} else
			throw std::overflow_error("");
	} catch(const std::overflow_error &e) {
		throw std::overflow_error("Multiply operator overflowed");
	}
	return r;
}

Fixed	Fixed::operator/(const Fixed &other) const {
	Fixed	r;

	if (other._bits == 0)
		throw std::invalid_argument("Division by zero");
	else {
		r._bits = toInt() * (1 << _point);
		int			iz = zeroes(r._bits);
		r._bits *= 1 << iz;
		r._bits /= other._bits;
		int			fract = (_bits - toInt() * (1 << _point));
		const int	fz = zeroes(fract);
		fract *= 1 << fz;
		fract /= other._bits;
		fract >>= (fz - iz);
		if (iz && ((r._bits > 0 && fract > std::numeric_limits<int>::max() - r._bits)
			|| (r._bits < 0 && fract < std::numeric_limits<int>::min() - r._bits))) {
			iz -= 1;
			r._bits /= 2;
			fract /= 2;
		}
		bool	overflowed = true;
		if (r._bits == 0 || fract == 0 ||
			(r._bits > 0 && fract <= std::numeric_limits<int>::max() - r._bits)
			|| (r._bits < 0 && fract >= std::numeric_limits<int>::min() - r._bits)) {
			r._bits += fract;
			r._bits >>= (iz > _point ? iz - _point : 0);
			const int	np = _point > iz ? _point - iz : 0;
			overflowed = false;
			if (np && ((r._bits > 0 && r._bits > std::numeric_limits<int>::max() >> np)
				|| (r._bits < 0 && r._bits < std::numeric_limits<int>::min() / (1 << np)))) {
				overflowed = true;
			} else
				r._bits *= 1 << np;
		}
		if (overflowed)
			throw std::overflow_error("Divide operator overflowed");
	}
	return r;
}

Fixed	&Fixed::operator++(void) {
	if (_bits < std::numeric_limits<int>::max())
		_bits += 1;
	else
		throw std::overflow_error("Pre-increment operator overflowed");
	return *this;
}

Fixed	Fixed::operator++(int) {
	Fixed	tmp(*this);

	if (_bits < std::numeric_limits<int>::max())
		_bits += 1;
	else
		throw std::overflow_error("Post-increment operator overflowed");
	return tmp;
}

Fixed	&Fixed::operator--(void) {
	if (_bits > std::numeric_limits<int>::min())
		_bits -= 1;
	else
		throw std::overflow_error("Pre-decrement operator overflowed");
	return *this;
}

Fixed	Fixed::operator--(int) {
	Fixed	tmp(*this);

	if (_bits > std::numeric_limits<int>::min())
		_bits -= 1;
	else
		throw std::overflow_error("Post-decrement operator overflowed");
	return tmp;
}

Fixed	&Fixed::min(Fixed &a, Fixed &b) {
	Fixed	*m;

	if (a <= b)
		m = &a;
	else
		m = &b;
	return *m;
}

const Fixed	&Fixed::min(const Fixed &a, const Fixed &b) {
	const Fixed	*m;

	if (a <= b)
		m = &a;
	else
		m = &b;
	return *m;
}

Fixed	&Fixed::max(Fixed &a, Fixed &b) {
	Fixed	*m;

	if (a >= b)
		m = &a;
	else
		m = &b;
	return *m;
}

const Fixed	&Fixed::max(const Fixed &a, const Fixed &b) {
	const Fixed	*m;

	if (a >= b)
		m = &a;
	else
		m = &b;
	return *m;
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
	return _bits / (1 <<_point);
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

const int	Fixed::_max = ~0u >> (_point + 1);

const int	Fixed::_min = ~0u << (sizeof(int) * 8 - _point - 1);

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

int	Fixed::zeroes(const int v) {
	int	z;
	if (v) {
		const ieee754_float	tmp = {static_cast<float>(v)};
		short	e = tmp.ieee.exponent;
		z = sizeof(int) * 8 - e
			+ std::numeric_limits<float>::max_exponent - 3;
	} else
		z = 23;
	return z;
}
