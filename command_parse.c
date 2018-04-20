#include "ls.h"

/*
**below is only for flag parsing and recording
*/

void    parse_flags(t_env *env, int argc, char **argv)
{
    int y;
    int x;

    y = 1;
    while(y < argc && argv[y][0] == '-')
    {
        x = 0;
        while (argv[y][++x] != '\0')
        {
            if (argv[y][x] == 'l')
                env->l = true;
            else if (argv[y][x] == '1')
                env->one = true;
            else if (argv[y][x] == 'a')
                env->a = true;
            else if (argv[y][x] == 'r')
                env->r = true;
            else if (argv[y][x] == 't')
                env->t = true;
            else if (argv[y][x] == 'R')
                env->R = true;
            else
            {   
                printf("ls: illegal option -- %c\n", argv[y][x]);
                printf("usage: ls [-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1] [file ...]"); 
                exit(0);
            }
        }
        y++;
    }
}

char    **file_names(int start, int argc, char **argv)
{
    char    **temp;
    int     i;

    temp = (char**)malloc(sizeof(char*) * (argc - start + 1));
    i = 0;
    while (start < argc)
    {
        temp[i] = ft_strdup(argv[start]);
        i++;
        start++;
    }
    temp[i] = NULL;
    return(temp);
}

/*
**below is only for 
*/

void    parse_dir(t_env *env, int argc, char **argv)
{
    int     y;
    char    **temp;

    y = 1;
    if (argc > 1)
    {
        while (y < argc && argv[y][0] == '-')
            y++;
    }
    if (y == argc)
    {
        temp = (char**)malloc(sizeof(char*) * 2);
        temp[0] = ft_strdup(".");
        temp[1] = NULL;
        env->dir = temp;
        return ;
    }
    temp = file_names(y, argc, argv);
    env->dir = temp;
}

void    parse_arg(t_env *env, int argc, char **argv)
{
    parse_flags(env, argc, argv);
    parse_dir(env, argc, argv);
}