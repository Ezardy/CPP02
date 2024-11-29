#ifndef EX00_FIXED_HPP
# define EX00_FIXED_HPP

class Fixed {
public:
	Fixed(void);
	Fixed(const Fixed &other);
	~Fixed(void);
	Fixed	&operator=(const Fixed &other);

	int		getRawBits(void) const;
	void	setRawBits(const int raw);
private:
	static const int	_point;
	int					_bits;
};
#endif
