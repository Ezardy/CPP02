#include <iostream>
#include <sstream>
#include <string>

#include "Point.hpp"

bool	bsp(Point const a, Point const b, Point const c, Point const point);

#define TEST_LOGIC_START(func_name) static bool func_name() {\
		bool				success = true;\
		const std::string	_func_name = #func_name;\
		std::streambuf		*old = std::cout.rdbuf();\
		std::ostringstream	oss;\
		std::string			expected;\
		std::cout << "[START " << _func_name << "]\n";\
		std::cout.rdbuf(oss.rdbuf());
#define TEST_LOGIC_END\
		std::cout.rdbuf(old);\
		success = success && expected == oss.str();\
		std::cout << oss.str();\
		if (!success) {\
			std::cout << "EXPECTED:\n" << expected;\
			std::cout << _func_name << " failed\n";\
		}\
		return success;\
	}

static bool	regular_trianlge(void);
static bool	flat_triangle(void);
static bool	point_rectangle(void);

int	main() {
	bool	success = true;
	bool	(*tests[])(void) = {
		regular_trianlge,
		flat_triangle,
		point_rectangle
	};
	size_t	tests_count = sizeof(tests) / sizeof(tests[0]);
	for (size_t i = 0; success && i < tests_count; i += 1) {
		success = tests[i]();
		std::cout << '\n';
	}
	if (success)
		std::cout << "OK\n";
	return success;
}

TEST_LOGIC_START(point_rectangle)
	const Point	a(5.5f, 23.5f);

	success = !bsp(a, a, a, a);
TEST_LOGIC_END

TEST_LOGIC_START(flat_triangle)
	const Point	a(0.0f, 0.0f);
	const Point	b(5.0f, 0.0f);
	const Point	c = b;

	success = !bsp(a, b, c, a) && !bsp(a, b, c, b)
		&& !bsp(a, b, c, Point(3.0f, 0.0f));
TEST_LOGIC_END

TEST_LOGIC_START(regular_trianlge)
	unsigned int	i;

	const Point		a(-2.5f, 0.0f);
	const Point		b(2.5f, -1.0f);
	const Point		c(-1.0f, 3.5f);

	const Point		pa = a;
	const Point		pb = b;
	const Point		pc = c;
	const Point		pca(-1.75f, 1.75f);
	const Point		pbc(0.75f, 1.25f);
	const Point		pab(1.25f, -0.75f);

	const Point		inside[] = {
		Point(0.5, 1.25),
		Point(0.75f, 1.0f),
		Point(1.0f, 0.75f),
		Point(1.25f, 0.5f),
		Point(1.25f, 0.25f),
		Point(1.5f, 0.0f),
		Point(1.75f, -0.25f),
		Point(2.0f, -0.5f),
		Point(2.0f, -0.75f),
		Point(1.5f, -0.5f),
		Point(1.25f, -0.5f),
		Point(1.0f, -0.5f),
		Point(0.75f, -0.5f),
		Point(0.5f, -0.5f),
		Point(0.25f, -0.25f),
		Point(0.0f, -0.25f),
		Point(-0.25f, -0.25f),
		Point(-0.5f, 0.0f),
		Point(-0.5f, -0.25f),
		Point(-0.75f, -0.25f),
		Point(-1.0f, 0.0f),
		Point(-1.25f, 0.0f),
		Point(-1.5f, 0.0f),
		Point(-1.75f, 0.0f),
		Point(-2.0f, 0.0f),
		Point(-2.25f, 0.0f),
		Point(-2.25f, 0.5f),
		Point(-2.0f, 0.75f),
		Point(-2.0f, 1.0f),
		Point(-1.75f, 1.25f),
		Point(-1.75f, 1.5f),
		Point(-1.5f, 1.75f),
		Point(-1.5f, 2.25f),
		Point(-1.25f, 2.5f),
		Point(-1.25f, 2.75f),
		Point(-1.0f, 3.25f),
		Point(-0.75f, 3.0f),
		Point(-0.5f, 2.75f),
		Point(-0.25f, 2.25f),
		Point(0.0f, 2.0f),
		Point(0.25f, 1.75f)
	};
	const Point		outside[] = {
		pa,
		pb,
		pc,
		pab,
		pbc,
		pca,
		Point(-0.25, 3.0f),
		Point(0.0f, 2.5f),
		Point(0.25f, 2.0f),
		Point(0.75f, 1.75f),
		Point(1.25f, 1.0f),
		Point(1.5f, 0.5f),
		Point(2.0f, 0.0f),
		Point(2.25f, -0.5f),
		Point(2.0f, -1.0f),
		Point(1.0f, -0.75f),
		Point(0.75f, -0.75f),
		Point(0.5f, -0.75f),
		Point(0.25f, -0.75f),
		Point(-0.25f, -0.75f),
		Point(-0.5f, -0.5f),
		Point(-0.75f, -0.5f),
		Point(-1.25f, -0.5f),
		Point(-1.5f, -0.25f),
		Point(-2.0f, -0.25f),
		Point(-2.5f, 0.5f),
		Point(-2.25f, 1.0f),
		Point(-2.25f, 1.25f),
		Point(-2.0f, 1.5f),
		Point(-1.75f, 2.25f),
		Point(-1.5f, 2.5f),
		Point(-1.5f, 3.0f),
		Point(-1.25f, 3.25f),
		Point(3.25f, 1.5f)
	};
	for (i = 0; success && i < sizeof(inside) / sizeof(inside[0]); i += 1) {
		if (!(success = bsp(a, b, c, inside[i])))
			std::cout << inside[i] << ' ' << i
				<< " expected to be inside the triangle, but it is not\n";
	}
	for (i = 0; success && i < sizeof(outside) / sizeof(outside[0]); i += 1) {
		if (!(success = bsp(a, b, c, outside[i]) == false))
			std::cout << outside[i] << ' ' << i
				<< " expected to be outside the triangle, but it is not\n";
	}
TEST_LOGIC_END
