
#ifndef LS_H
# define LS_H

# include <unistd.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <dirent.h>
# include <stdio.h>
# include <strings.h>
# include <stdbool.h>
# include "./libft/libft.h"

typedef struct      s_cont
{
    char            *path;
    struct stat     cont;
    
    struct s_cont   *next;
    struct s_cont   *last;
}                   t_cont;

typedef struct          s_opndir
{
    char                *path;
    DIR                 *dir;
    struct dirent       *dirent;
    
    struct  s_cont      *dir_cont;

    struct s_opndir    *next;
    struct s_opndir    *last;
}                       t_opndir;


typedef struct          s_env
{
    bool                l;
    bool                a;
    bool                t;
    bool                R;
    bool                r;
    bool                one;
    char                **dir;
}                       t_env;

void    print_list(t_opndir *s);
void    parse_arg(t_env *env, int argc, char **argv);
t_opndir    *assemble_first_queue(t_env *env);
t_cont      *insert_cont(char *path, t_cont *head, struct stat *buffer);
t_opndir    *insert_dir(char *path, t_opndir *head);
t_opndir    *opndir_new(char *path, t_opndir *next, t_opndir *last);
t_cont      *cont_new(char *path, t_cont *next, t_cont *last, struct stat *buffer);
t_opndir    *time_assemble_sort(t_env *env);




#endif