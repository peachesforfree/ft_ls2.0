#include "ft_ls.h"

int     check_flags(char *flags, int argc, char **argv)
{
    (void)argc;
    int     result;
    int     y;
    int     x;
    
    result = 0;
    y = 1;
    while (argv[y] != NULL)
    {
        x = 0;
        if (argv[y][0] == '-')
        {
            while (argv[y][x] != '\0' && ft_strchr(flags, argv[y][x]))
            {
                if (argv[y][x] == 'l')
                    result |= LONGFLG;
                if (argv[y][x] == 'R')
                    result |= RECFLG;
                if (argv[y][x] == 'a')
                    result |= HIDFLG;
                if (argv[y][x] == 'r')
                    result |= REVFLG;
                if (argv[y][x] == 't')
                    result |= TIMFLG;
                x++;
            }
        }
        else if (!ft_strchr(flags, argv[y][x]))
            return (result);
        y++;
    }
    return (result);
}

void    print_flags(int flags)
{
    if (flags & LONGFLG)
        printf("\tlong flag found\n");
    if (flags & RECFLG)
        printf("\trecursive flag found\n");
    if (flags & HIDFLG)
        printf("\thidden flag found\n");
    if (flags & REVFLG)
        printf("\tReverse flag found\n");
    if (flags & TIMFLG)
        printf("\ttime flag found\n");
}

int     main(int argc, char **argv)
{
    int     flags;
    flags = check_flags(FLAGCHAR, argc, argv);
    print_flags(flags); //for testing
    return(0);
}