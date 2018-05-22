#ifndef LS_H
# define LS_H

# include <unistd.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <dirent.h>
# include <strings.h>
# include <stdbool.h>
# include <stdio.h>
# include "libft/libft.h"

# define FLAGCHAR "-lRart"
# define LONGFLG    0b00000001
# define RECFLG     0b00000010
# define HIDFLG     0b00000100
# define REVFLG     0b00001000
# define TIMFLG     0b00010000

/* bonus
-u -f -g -d 
and or handle spacing for long list on window
*/

typedef struct          s_opndir
{
    char                *path;
    DIR                 *dir;
    struct dirent       *dirent;

    struct  s_cont      *dir_cont;

    struct s_opndir    *next;
    struct s_opndir    *last;
}                       t_opndir;

//use as guide for flag markers 
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



#endif