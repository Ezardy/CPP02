#include <iostream>

#include "Fixed.hpp"

Fixed::Fixed(void) : _bits(0) {
	std::cout << "Default constructor called\n";
}

Fixed::Fixed(const Fixed &other) {
	std::cout << "Copy constructor called\n";
	*this = other;
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

const int	Fixed::_point = 8;
