#include "Point.hpp"

Point::Point(void) {

}

Point::Point(const float x, const float y) : x(x), y(y) {

}

Point::Point(const Fixed x, const Fixed y) : x(x), y(y) {

}

Point::Point(const Point &other) : x(other.x), y(other.y) {

}

Point	&Point::operator=(const Point &other) {
	(void)other;
	return *this;
}

Point::~Point(void) {

}

Point	Point::operator-(const Point &other) const {
	return Point(x - other.x, y - other.y);
}

Fixed	Point::operator*(const Point &other) const {
	return x * other.y - other.x * y;
}

std::ostream	&operator<<(std::ostream &os, const Point &point) {
	os << '(' << point.getX() << ' ' << point.getY() << ')';
	return os;
}

const Fixed	&Point::getX(void) const {
	return x;
}

const Fixed	&Point::getY(void) const {
	return y;
}
