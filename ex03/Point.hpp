#ifndef EX03_POINT_HPP
# define EX03_POINT_HPP

# include <iostream>

# include "Fixed.hpp"

class Point {
public:
	Point(void);
	Point(const float x, const float y);
	Point(const Fixed x, const Fixed y);
	Point(const Point &other);
	Point		&operator=(const Point &other);
	~Point(void);

	Point		operator-(const Point &other) const;
	Fixed		operator*(const Point &other) const;

	const Fixed	&getX(void) const;
	const Fixed	&getY(void) const;
private:
	const Fixed	x;
	const Fixed	y;
};

std::ostream	&operator<<(std::ostream &os, const Point &point);

#endif
