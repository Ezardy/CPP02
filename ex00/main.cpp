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

static bool	copy_assignment(void);
static bool	set_raw_bits(void);
static bool	default_test(void);

int	main() {
	bool	success = true;
	bool	(*tests[])(void) = {
		copy_assignment,
		set_raw_bits,
		default_test
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

TEST_LOGIC_START(default_test)
	{
		Fixed a;
		Fixed b( a );
		Fixed c;
		c = b;
		std::cout << a.getRawBits() << std::endl;
		std::cout << b.getRawBits() << std::endl;
		std::cout << c.getRawBits() << std::endl;
	}
	expected = "Default constructor called\n"
		"Copy constructor called\n"
		"Copy assignment operator called\n"
		"getRawBits member function called\n"
		"Default constructor called\n"
		"Copy assignment operator called\n"
		"getRawBits member function called\n"
		"getRawBits member function called\n"
		"0\n"
		"getRawBits member function called\n"
		"0\n"
		"getRawBits member function called\n"
		"0\n"
		"Destructor called\n"
		"Destructor called\n"
		"Destructor called\n";
TEST_LOGIC_END

TEST_LOGIC_START(set_raw_bits)
	{
		Fixed	a;
		a.setRawBits(124);
		Fixed	b = a;
		Fixed	c;
		c = b;
		std::cout << a.getRawBits() << b.getRawBits() << c.getRawBits() << '\n';
	}
	expected = "Default constructor called\n"
		"setRawBits member function called\n"
		"Copy constructor called\n"
		"Copy assignment operator called\n"
		"getRawBits member function called\n"
		"Default constructor called\n"
		"Copy assignment operator called\n"
		"getRawBits member function called\n"
		"getRawBits member function called\n"
		"124getRawBits member function called\n"
		"124getRawBits member function called\n124\nDestructor called\n"
		"Destructor called\nDestructor called\n";
TEST_LOGIC_END

TEST_LOGIC_START(copy_assignment)
	{
		Fixed	a;
		Fixed	b = a;
		Fixed	&ra = a;
		a = ra;
	}
	expected = "Default constructor called\n"
		"Copy constructor called\n"
		"Copy assignment operator called\n"
		"getRawBits member function called\n"
		"Destructor called\nDestructor called\n";
TEST_LOGIC_END
