#include <unistd.h>

int		len(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int main(int argc, char **argv)
{
	int i;

	i = 0;
	while (i < argc)
	{
		write(1, argv[i], len(argv[i]));
		i++;
	}
	return (0);
}
