#include "Point.hpp"

bool	bsp(Point const a, Point const b, Point const c, Point const point) {
	const Fixed	zero;

	return (b - a) * (point - a) > zero && (c - b) * (point - b) > zero
		&& (a - c) * (point - c) > zero;
}
