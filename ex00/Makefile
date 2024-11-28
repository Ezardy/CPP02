all: test

src=Fixed main
obj=$(addsuffix .o, $(src))
iflags=-Wall -Wextra -Werror -std=c++98
command=c++ $(iflags) -o $@ -c $<

test: $(obj)
	c++ -o $@ $(obj)

Fixed.o: Fixed.cpp Fixed.hpp Makefile
	$(command)

main.o: main.cpp Fixed.hpp Makefile
	$(command)

clean:
	rm -f $(obj)

fclean: clean
	rm -f test

re: fclean test