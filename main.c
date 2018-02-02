//#include "ft_ls.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

typedef struct  s_cont
{
    struct dirent   *cont;

    struct s_cont   *next;
    struct s_cont   *last;
}               t_cont;

typedef struct          s_opndir
{
    DIR                 *dir;
    struct  s_cont      *dp;

    struct s_opndir    *next;
    struct s_opndir    *last;
}                       t_opndir;

int     err(char *str)
{
    printf("Sorry can not open '%s'", str);
    return (0);
}

t_cont  *cont_new(t_cont *next, t_cont *last)
{
    t_cont  *temp;

    temp = (t_cont*)malloc(sizeof(t_cont) * 1);
    bzero(temp, sizeof(t_cont));
    if (next != NULL)
        temp->next = next;
    if (last != NULL)
        temp->last = last;
    return (temp);
}

t_opndir    *opndir_new(t_opndir *next, t_opndir *last)
{
    t_opndir    *temp;

    temp = (t_opndir*)malloc(sizeof(t_opndir) * 1);
    bzero(temp, sizeof(t_opndir));
    if (next != NULL)
        temp->next = next;
    if (last != NULL)
        temp->last = last;
    return (temp);
}

t_opndir    *opndir_init(void)
{
    t_opndir *temp;

    temp = opndir_new(NULL, NULL);
    temp->dp = cont_new(NULL, NULL);
    return (temp);
}

void    list_contents(t_opndir *s)
{
    t_cont  *temp_cont;

    temp_cont = s->dp;
    while ((temp_cont->cont = readdir(s->dir)) != NULL)
    {
        if (temp_cont->last == NULL)
            cont_new(NULL, temp_cont);
        else
            cont_new(temp_cont, NULL);
        temp_cont = temp_cont->next;
    }
    //sort in alphabetical order
    //sort in reverse alphabetical order
    while (temp_cont != NULL)
    {
        printf("%s\n", temp_cont->cont->d_name);
        temp_cont = temp_cont->next;
    }
}


void    open_directory(char *path)
{
    t_opndir    *s;

    s = opndir_init();
    if ((s->dir = opendir(path)) == NULL)
    {
        free(s);
        err(".");
        return ;
    }
    list_contents(s);
    closedir(s->dir);
    //make a loop to check if any of the contents is a directory, if so and if -R flag call open_directory again
}


int     main(int argc, char **argv)
{
    open_directory(".");
    return (0);
}

