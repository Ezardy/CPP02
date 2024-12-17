all: test

src=main bsp Point Fixed
obj=$(addsuffix .o, $(src))

test: $(obj)
	c++ -o $@ $(obj)

iflags=-Wall -Werror -Wextra -std=c++98
command=c++ $(iflags) -o $@ -c $<

main.o: main.cpp Point.hpp Fixed.hpp ieee754.h Makefile
	$(command)

bsp.o: bsp.cpp Point.hpp Fixed.hpp ieee754.h Makefile
	$(command)

Fixed.o: Fixed.cpp Fixed.hpp ieee754.h Makefile
	$(command)

Point.o: Point.cpp Point.hpp Fixed.hpp ieee754.h Makefile
	$(command)

clean:
	rm -f $(obj)

fclean: clean
	rm -f test

re: fclean test

debug: iflags+=-g
debug: test