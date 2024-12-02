#include <iostream>
#include <sstream>
#include <string>

#include "Fixed.hpp"

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

static bool	lt(void);
static bool	le(void);
static bool	gt(void);
static bool	ge(void);
static bool	eq(void);
static bool	ne(void);

static bool	add(void);
static bool	subtract(void);
static bool	multiply(void);
static bool	divide(void);

static bool	pre_inc(void);
static bool	post_inc(void);
static bool	pre_dec(void);
static bool	post_dec(void);

static bool	min(void);
static bool	max(void);

static bool	ex02_default(void);

int	main() {
	bool	success = true;
	bool	(*tests[])(void) = {
		lt,
		le,
		gt,
		ge,
		eq,
		ne,
		add,
		subtract,
		multiply,
		divide,
		pre_inc,
		post_inc,
		pre_dec,
		post_dec,
		min,
		max,
		ex02_default
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

TEST_LOGIC_START(ex02_default)
	Fixed a;
	Fixed const b( Fixed( 5.05f ) * Fixed( 2 ) );

	std::cout << a << std::endl;
	std::cout << ++a << std::endl;
	std::cout << a << std::endl;
	std::cout << a++ << std::endl;
	std::cout << a << std::endl;

	std::cout << b << std::endl;

	std::cout << Fixed::max( a, b ) << std::endl;

	expected = "0\n"
		"0.00390625\n"
		"0.00390625\n"
		"0.00390625\n"
		"0.0078125\n"
		"10.1016\n"
		"10.1016\n";
TEST_LOGIC_END

TEST_LOGIC_START(max)
	Fixed	a(-0.00390625f);
	Fixed	b(0.00390625f);
	Fixed	c = a;

	std::cout << Fixed::max(a, b) << ' ' << Fixed::max(b, a) << ' '
		<< Fixed::max(a, c) << '\n';

	expected = "0.00390625f 0.00390625f 0.00390625f\n";
TEST_LOGIC_END

TEST_LOGIC_START(min)
	Fixed	a(-0.00390625f);
	Fixed	b(0.00390625f);
	Fixed	c = a;

	std::cout << Fixed::min(a, b) << ' ' << Fixed::min(b, a) << ' '
		<< Fixed::min(a, c) << '\n';

	expected = "-0.00390625f -0.00390625f -0.00390625f\n";
TEST_LOGIC_END

TEST_LOGIC_START(post_dec)
	Fixed	a(-1);
	Fixed	c(0);

	std::cout << a-- << ' ' << a << '\n';
	std::cout << c-- << ' ' << c << '\n';
	try {
		Fixed	b(-8388608);
		std::cout << b-- << ' ' << b << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}

	expected = "-1 -1.00390625\n"
		"0 -0.00390625"
		"Post-decrement operator overflowed\n";
TEST_LOGIC_END

TEST_LOGIC_START(pre_dec)
	Fixed	a(-1);
	Fixed	c(0);

	std::cout << --a << ' ' << a << '\n';
	std::cout << --c << ' ' << c << '\n';
	try {
		Fixed	b(-8388608);
		std::cout << --b << ' ' << b << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}

	expected = "-1.00390625 -1.00390625\n"
		"-0.00390625 -0.00390625"
		"Pre-decrement operator overflowed\n";
TEST_LOGIC_END

TEST_LOGIC_START(post_inc)
	Fixed	a(1);
	Fixed	c(-0.00390625f);

	std::cout << a++ << ' ' << a << '\n';
	std::cout << c++ << ' ' << c << '\n';
	try {
		Fixed	b(8388607);
		std::cout << b++ << ' ' << b << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}

	expected = "1 1.00390625\n"
		"-0.00390625 0"
		"Post-increment operator overflowed\n";
TEST_LOGIC_END

TEST_LOGIC_START(pre_inc)
	Fixed	a(1);
	Fixed	c(-0.00390625f);

	std::cout << ++a << ' ' << a << '\n';
	std::cout << ++c << ' ' << c << '\n';
	try {
		Fixed	b(8388607);
		std::cout << ++b << ' ' << b << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}

	expected = "1.00390625 1.00390625\n"
		"0 0\n"
		"Pre-increment operator overflowed\n";
TEST_LOGIC_END

TEST_LOGIC_START(divide)
	std::cout << (Fixed(0) / Fixed(0.00390625f)) << '\n';
	std::cout << (Fixed(0) / Fixed(-0.00390625f)) << '\n';
	std::cout << (Fixed(1.0f) / (Fixed(-0.00390625f))) << '\n';
	std::cout << (Fixed(32768.0f) / (Fixed(-0.00390625f))) << '\n';
	std::cout << (Fixed(-2352.625f) / Fixed(1.0f)) << '\n';
	std::cout << (Fixed(-2352.625f) / Fixed(-1)) << '\n';
	std::cout << (Fixed(37836) / Fixed(7.125f)) << '\n';
	try {
		std::cout << (Fixed(0.0f) / Fixed(0)) << '\n';
	} catch (const std::invalid_argument &e) {
		std::cout << e.what() << '\n';
	}
	try {
		std::cout << (Fixed(235.2352f) / Fixed(0.0f)) << '\n';
	} catch (const std::invalid_argument &e) {
		std::cout << e.what() << '\n';
	}
	try {
		std::cout << (Fixed(32768.0f) / (Fixed(0.00390625f))) << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}
	try {
		std::cout << (Fixed(8388607) / (Fixed(0.99999f))) << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}

	expected = "0\n"
		"0\n"
		"-256\n"
		"-8388608\n"
		"-2352.62\n"
		"2352.62\n"
		"5310.32\n"
		"Division by zero\n"
		"Division by zero\n"
		"Divide operator overflowed\n"
		"Divide operator overflowed\n";
TEST_LOGIC_END

TEST_LOGIC_START(multiply)
	std::cout << (Fixed(50.625f) * Fixed(-50.5f)) << '\n';
	std::cout << (Fixed(8388607) * Fixed(0)) << '\n';
	std::cout << (Fixed(0) * Fixed(8388607)) << '\n';
	std::cout << (Fixed(8388607) * Fixed(1)) << '\n';
	std::cout << (Fixed(1) * Fixed(8388607)) << '\n';
	std::cout << (Fixed(-1) * Fixed(8388607)) << '\n';
	std::cout << (Fixed(-500.125f) * Fixed(-0.5f)) << '\n';
	std::cout << (Fixed(8388607) * Fixed(0.5f)) << '\n';
	std::cout << (Fixed(-8388608) * Fixed(0.5f)) << '\n';
	std::cout << (Fixed(-8388608) * Fixed(-0.5f)) << '\n';
	try {
		std::cout << (Fixed(-8388608) * Fixed(-1)) << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}
	try {
		std::cout << (Fixed(-8388608) * Fixed(1.00390625f)) << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}
	try {
		std::cout << (Fixed(8388607) * Fixed(1.00390625f)) << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}
	try {
		std::cout << (Fixed(-8388608) * Fixed(8388608)) << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}

	expected =  "-2556.56"
		"0\n"
		"0\n"
		"8388607\n"
		"8388607\n"
		"-8388607\n"
		"250.062\n"
		"4.1943e+06\n"
		"-4194304\n"
		"4194304\n"
		"Multiply operator overflowed\n"
		"Multiply operator overflowed\n"
		"Multiply operator overflowed\n"
		"Multiply operator overflowed\n";
TEST_LOGIC_END

TEST_LOGIC_START(subtract)
	std::cout << (Fixed(8388607) - Fixed(1)) << '\n';
	std::cout << (Fixed(-8388608) - (Fixed(-1))) << '\n';
	std::cout << (Fixed(345) - Fixed(500.625f)) << '\n';
	try {
		std::cout << (Fixed(-8388608) - Fixed(1)) << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}
	try {
		std::cout << (Fixed(8388607) - Fixed(-1)) << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}
	try {
		std::cout << (Fixed(-8388608) - Fixed(0.00390625f)) << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}
	try {
		std::cout << (Fixed(8388606) - Fixed(-1.00390625f)) << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}

	expected = "8388606\n"
		"-8388607\n"
		"-154.375\n"
		"Subtract operator overflowed\n"
		"Subtract operator overflowed\n"
		"Subtract operator overflowed\n"
		"Subtract operator overflowed\n";
TEST_LOGIC_END

TEST_LOGIC_START(add)
	std::cout << (Fixed(-8388608) + Fixed(8388607)) << '\n';
	std::cout << (Fixed(0) + Fixed(0)) << '\n';
	std::cout << (Fixed(0.125f) + Fixed(999.875f)) << '\n';
	try {
		std::cout << (Fixed(-8388608) + Fixed(-1)) << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}
	try {
		std::cout << (Fixed(8388607) + Fixed(1)) << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}
	try {
		std::cout << (Fixed(8388607) + Fixed(0.00390625f)) << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}
	try {
		std::cout << (Fixed(-8388607) + Fixed(-1.00390625f)) << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}

	expected = "-1\n"
		"0\n"
		"1000\n"
		"Add operator overflowed\n"
		"Add operator overflowed\n"
		"Add operator overflowed\n"
		"Add operator overflowed\n";
TEST_LOGIC_END

TEST_LOGIC_START(ne)
	Fixed	small_pos(0.00390625f);
	Fixed	small_neg(-0.00390625f);
	Fixed	big_pos(8388607);
	Fixed	other_big_pos = big_pos;
	Fixed	big_neg(-8388608);
	Fixed	zero(0);

	std::cout << (small_neg != small_pos) << ' ' << (zero != small_neg) << ' '
		<< (big_neg != big_pos) << ' ' << (zero != zero) << ' '
		<< (big_pos != other_big_pos) << '\n';
	expected = "true true true false false\n";
TEST_LOGIC_END

TEST_LOGIC_START(eq)
	Fixed	small_pos(0.00390625f);
	Fixed	small_neg(-0.00390625f);
	Fixed	big_pos(8388607);
	Fixed	other_big_pos = big_pos;
	Fixed	big_neg(-8388608);
	Fixed	zero(0);

	std::cout << (small_neg == small_pos) << ' ' << (zero == small_neg) << ' '
		<< (big_neg == big_pos) << ' ' << (zero == zero) << ' '
		<< (big_pos == other_big_pos) << '\n';
	expected = "false false false true true\n";
TEST_LOGIC_END

TEST_LOGIC_START(ge)
	Fixed	small_pos(0.00390625f);
	Fixed	small_neg(-0.00390625f);
	Fixed	big_pos(8388607);
	Fixed	big_neg(-8388608);
	Fixed	zero(0);

	std::cout << (small_neg > small_pos) << ' ' << (zero > small_neg) << ' '
		<< (big_neg > big_pos) << ' ' << (zero > zero) << '\n';
	expected = "false true false true\n";
TEST_LOGIC_END

TEST_LOGIC_START(gt)
	Fixed	small_pos(0.00390625f);
	Fixed	small_neg(-0.00390625f);
	Fixed	big_pos(8388607);
	Fixed	big_neg(-8388608);
	Fixed	zero(0);

	std::cout << (small_neg > small_pos) << ' ' << (zero > small_neg) << ' '
		<< (big_neg > big_pos) << ' ' << (zero > zero) << '\n';
	expected = "false true false false\n";
TEST_LOGIC_END

TEST_LOGIC_START(le)
	Fixed	small_pos(0.00390625f);
	Fixed	small_neg(-0.00390625f);
	Fixed	big_pos(8388607);
	Fixed	big_neg(-8388608);
	Fixed	zero(0);

	std::cout << (small_neg <= small_pos) << ' ' << (zero <= small_neg) << ' '
		<< (big_neg <= big_pos) << ' ' << (zero <= zero) << '\n';
	expected = "true false true true\n";
TEST_LOGIC_END

TEST_LOGIC_START(lt)
	Fixed	small_pos(0.00390625f);
	Fixed	small_neg(-0.00390625f);
	Fixed	big_pos(8388607);
	Fixed	big_neg(-8388608);
	Fixed	zero(0);

	std::cout << (small_neg < small_pos) << ' ' << (zero < small_neg) << ' '
		<< (big_neg < big_pos) << ' ' << (zero < zero) << '\n';
	expected = "true false true false\n";
TEST_LOGIC_END
