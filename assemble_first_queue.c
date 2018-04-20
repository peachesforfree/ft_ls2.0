#include "ls.h"

/*
**This inserts data in reverse alphabetical order in
**a linked list at opndir->dir_cont
*/

t_cont      *insert_cont(char *path, t_cont *head, struct stat *buffer)
{
    t_cont  *current;

    current = head;
    if (head == NULL)
        return (cont_new(path, NULL, NULL, buffer));
    while (current)
    {
        if (ft_strcmp(current->path, path) == 0)
            break;
        if (current->next && ft_strcmp(current->path, path) > 0 && ft_strcmp(current->next->path, path) < 0)
        {    
            current->next = cont_new(path, current->next, current, buffer);
            current->next->next->last = current->next;
            break ;
        }
        if (ft_strcmp(current->path, path) < 0) //goes before list
            return (cont_new(path, head, NULL, buffer));
        if (current->next == NULL)
        {
            current->next = cont_new(path, NULL, current, buffer);
            break ;
        }
        current = current->next;
    }
    return (head);
}

/*
**while keeping track of the head
**this inserts new opn_dir items in alpha order
*/

t_opndir    *assemble_first_queue(t_env *env)
{
    t_opndir        *head;
    struct stat     buffer;
    int             i;

    i = 0;
    if (env->t == true)
        return (time_assemble_sort(env));
    head = opndir_new(NULL, NULL, NULL);
    while (env->dir[i])
    {
        if (lstat(env->dir[i], &buffer) == 0)         //tries to open the file/directory
        {
            if(S_ISDIR(buffer.st_mode))                 //used the st_mode to tell if file or directory
                head = insert_dir(env->dir[i], head); //place into DIR queue
            if(S_ISREG(buffer.st_mode))
                head->dir_cont = insert_cont(env->dir[i], head->dir_cont, &buffer);   //place in dir_cont queue
        }
        else
            printf("ls: %s: No such file or directory\n", env->dir[i]);
        i++;
    }
    return(head);
}