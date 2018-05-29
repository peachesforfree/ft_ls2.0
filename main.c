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

t_cont      *new_cont(char *path, t_cont *before, t_cont *after)
{
    t_cont *temp;

    temp = (t_cont*)ft_memalloc(sizeof(t_cont));
    if (path)
        temp->path = ft_strdup(path);

        //maybe run lstat here ?
        //maybe also check permnissions if needed ?


    if (before != NULL)
    {
        temp->last = before;
        before->next = temp;
    }
    if (after != NULL)
    {
        temp->next = after;
        after->last = temp;
    }
    return (temp);
}

/***************************************************************
 * problem is right here, need to figure out how to properly sort 
 * and insert in ascii order
 * ***************************************************************/


t_cont        *insert_alpha(char *path, t_cont *head)
{
    t_cont  *current;

    //if first item in list, this will make it the head;
    if (head == NULL)
        return (new_cont(path, NULL, NULL));
    current = head;
    while (current)
    {
        //if comparison makes eval cont to next link
        
        //if need to place in front of list
        if (current->last == NULL)
            return (new_cont(path, NULL, current));
        //if need to place in between links
        else if (ft_strcmp(current->path, path) < 0)
        {
            new_cont(path, current, current->next);
            break;
        }
        else if (ft_strcmp(current->path, path) > 0)
        {
            current = current->next;
            continue;
        }
    }
    return (head);
}

/*
**this assembles the directory list chain
*/

t_cont        *add_cont(char *path, t_cont *head, int flags)
{
    (void)flags;
//    if (flags & TIMFLG)       NEED TO MAKE
//        return(insert_time(path, head));
//    else
        return(insert_alpha(path, head));
}

/*
**To assemble first queue. Either assembles the list of stated items
** OR makes current directory the only item in head list
*/

t_opndir    *start_queue(int flags, char **argv)
{
    t_opndir    *result;
    int y;

    y = 1;
    //allocates memory for first list
    result = (t_opndir*)ft_memalloc(sizeof(t_opndir));
    //jumps through the flag statements
    while (argv[y] != NULL && argv[y][0] == '-')
        y++;
    //if there are no specififed files/directories
    if (y == 1 && argv[y] == NULL)
      result->dir_cont = new_cont(".", NULL, NULL);
    //else if there are other arguemnts in list and its not at end
    //go into making the directory content chain and put user items in order
    else
    {
        while (argv[y] != NULL)
        {
            result->dir_cont = add_cont(argv[y], result->dir_cont, flags);
            y++;
        }
    }
    return (result);
}

t_opndir    *print_dir_cont(t_opndir *head, int flags)
{
    t_cont  *current;
    (void)flags;
    
    current = head->dir_cont;
    while (current != NULL)
    {
        printf("\t%s\n", current->path);
        current = current->next;
    }
    return(head->next);
}

int     main(int argc, char **argv)
{
    int         flags;
    t_opndir    *head;

    flags = check_flags(FLAGCHAR, argc, argv);
    //print_flags(flags); //for testing
    head = start_queue(flags, argv);
    while (head)
        head = print_dir_cont(head, flags);
    return(0);
}