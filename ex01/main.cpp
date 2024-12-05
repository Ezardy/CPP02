#include <iostream>
#include <sstream>
#include <string>
#include <limits>

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

static bool	ex01_default_test(void);
static bool	negatives(void);
static bool	float_special_cases(void);
static bool	int_overflows(void);
static bool	float_overflows(void);
static bool	zeroes(void);
static bool	rounding(void);

int	main() {
	bool	success = true;
	bool	(*tests[])(void) = {
		negatives,
		float_special_cases,
		int_overflows,
		float_overflows,
		zeroes,
		ex01_default_test,
		rounding
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

TEST_LOGIC_START(rounding)
	Fixed	a;
	a.setRawBits(std::numeric_limits<int>::max());
	std::cout << a << '\n';


	success = a.toFloat() == 8388608;
	expected = "Default constructor called\n8.38861e+06\n";
TEST_LOGIC_END

TEST_LOGIC_START(zeroes)
	Fixed	a(0);
	Fixed	b(0.0f);
	Fixed	d(0.00390624f);
	Fixed	c(0.00390625f);

	std::cout << a << ' ' << b << ' ' << d << ' ' << c << '\n';

	expected = "Int constructor called\n"
		"Float constructor called\n"
		"Float constructor called\n"
		"Float constructor called\n"
		"0 0 0 0.00390625\n";
TEST_LOGIC_END

TEST_LOGIC_START(float_overflows)
	try {
		Fixed	a(-8388608.6f);
		std::cout << a << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}
	try {
		Fixed	a(8388607.8f);
		std::cout << a << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}
	Fixed	a(8388607.5f);
	std::cout << a << '\n';

	expected = "Presented float is out of the Fixed type's range\n"
		"Presented float is out of the Fixed type's range\n"
		"Float constructor called\n8.38861e+06\n";
TEST_LOGIC_END

TEST_LOGIC_START(int_overflows)
	try {
		Fixed	a(-8388609);
		std::cout << a << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}
	try {
		Fixed	a(8388608);
		std::cout << a << '\n';
	} catch (const std::overflow_error &e) {
		std::cout << e.what() << '\n';
	}
	Fixed	a(8388607);
	std::cout << a << '\n';

	expected = "The integer is too big for the Fixed type\n"
		"The integer is too big for the Fixed type\n"
		"Int constructor called\n8388607\n";
TEST_LOGIC_END

TEST_LOGIC_START(float_special_cases)
	try {
		const Fixed	a(-std::numeric_limits<float>::quiet_NaN());
	} catch (const std::invalid_argument &e) {
		std::cout << e.what() << '\n';
	}
	try {
		const Fixed	a(std::numeric_limits<float>::quiet_NaN());
	} catch (const std::invalid_argument &e) {
		std::cout << e.what() << '\n';
	}
	try {
		const Fixed	a(-std::numeric_limits<float>::signaling_NaN());
	} catch (const std::invalid_argument &e) {
		std::cout << e.what() << '\n';
	}
	try {
		const Fixed	a(std::numeric_limits<float>::signaling_NaN());
	} catch (const std::invalid_argument &e) {
		std::cout << e.what() << '\n';
	}
	try {
		const Fixed	a(std::numeric_limits<float>::infinity());
	} catch (const std::invalid_argument &e) {
		std::cout << e.what() << '\n';
	}
	try {
		const Fixed	a(-std::numeric_limits<float>::infinity());
	} catch (const std::invalid_argument &e) {
		std::cout << e.what() << '\n';
	}

	expected = "Presented float is NAN or INF\n"
		"Presented float is NAN or INF\n"
		"Presented float is NAN or INF\n"
		"Presented float is NAN or INF\n"
		"Presented float is NAN or INF\n"
		"Presented float is NAN or INF\n";
TEST_LOGIC_END

TEST_LOGIC_START(negatives)
	const Fixed	a(-8388608);
	const Fixed	b(-8388608.0f);
	const Fixed	c(-5);

	std::cout << a << ' ' << b << ' ' << c << '\n';

	expected = "Int constructor called\nFloat constructor called\n"
		"Int constructor called\n-8388608 -8388608 -5\n";
TEST_LOGIC_END

TEST_LOGIC_START(ex01_default_test) {
	Fixed	a;
	Fixed	const b( 10 );
	Fixed	const c( 42.42f );
	Fixed	const d( b );

	a = Fixed( 1234.4321f );

	std::cout << "a is " << a << std::endl;
	std::cout << "b is " << b << std::endl;
	std::cout << "c is " << c << std::endl;
	std::cout << "d is " << d << std::endl;

	std::cout << "a is " << a.toInt() << " as integer" << std::endl;
	std::cout << "b is " << b.toInt() << " as integer" << std::endl;
	std::cout << "c is " << c.toInt() << " as integer" << std::endl;
	std::cout << "d is " << d.toInt() << " as integer" << std::endl;
}
	expected = "Default constructor called\n"
		"Int constructor called\n"
		"Float constructor called\n"
		"Copy constructor called\n"
		"Copy assignment operator called\n"
		"Float constructor called\n"
		"Copy assignment operator called\n"
		"Destructor called\n"
		"a is 1234.43\n"
		"b is 10\n"
		"c is 42.4219\n"
		"d is 10\n"
		"a is 1234 as integer\n"
		"b is 10 as integer\n"
		"c is 42 as integer\n"
		"d is 10 as integer\n"
		"Destructor called\n"
		"Destructor called\n"
		"Destructor called\n"
		"Destructor called\n";
TEST_LOGIC_END
