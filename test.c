#include "libft/libft.h"
#include <stdio.h>

int		main(int argc, char **argv)
{
	printf("%s\t%s\t= %d", argv[1], argv[2], ft_strcmp(argv[2], argv[1]));
	return (0);
}
