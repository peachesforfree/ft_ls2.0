#include "ft_ls.h"

void    error_no_option(char c)
{
    printf("ls: illegal option -- %c\nusage: ls [%s] [file ...]", c, FLAGCHAR);
    exit(0);
}

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
            x++;
            while (argv[y][x] != '\0' && ft_strchr(FLAGCHAR, argv[y][x]))
            {
                if (argv[y][x] == 'l')
                    result |= LONGFLG;
                else if (argv[y][x] == 'R')
                    result |= RECFLG;
                else if (argv[y][x] == 'a')
                    result |= HIDFLG;
                else if (argv[y][x] == 'r')
                    result |= REVFLG;
                else if (argv[y][x] == 't')
                    result |= TIMFLG;
                x++;
            }
            if (argv[y][x] != '\0' && (argv[y][0] == '-') && !ft_strchr(FLAGCHAR, argv[y][x]))
                error_no_option(argv[y][x]);
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
        if (ft_strcmp(current->path, path) >= 0)
        {
            if (current->last == NULL)
            {
                head = new_cont(path, NULL, current);
                break;
            }
            else
            {
                new_cont(path, current->last, current);
                break;
            }
        }
        else if (ft_strcmp(current->path, path) < 0)
        {
            if (current->next == NULL)
            {
                new_cont(path, current, NULL);
                break;
            }
        }
        current = current->next;
    }
    return (head);
}

/*
**this assembles the directory list chain
*/

t_cont        *add_cont(char *path, t_cont *head, int flags)
{
    (void)flags;
//    if (flags & TIMFLG)
//        return(insert_time(path, head));
//    else
        return(insert_alpha(path, head));
}

t_opndir        *new_dir(char *path)
{
    t_opndir    *temp;

    temp = ft_memalloc(sizeof(t_opndir));
    temp->path = ft_strdup(path);
    return (temp);
}

void        stack_opndir(t_opndir  *current, t_opndir *new)
{
    t_opndir    *temp;

    temp = NULL;
    if(current->next != NULL)
        temp = current->next;
    current->next = new;
    new->last = current;
    if (temp != NULL)
    {
        temp->last = new;
        new->next = temp;
    } 
}

void        enqueue_dir(t_opndir *head, t_opndir *new)
{
    t_opndir    *temp;

    temp = head;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = new;
    new->last = temp;
}


void            build_first_directory_chain(t_opndir *head)
{
    t_cont      *temp;
    t_opndir    *current;
    t_opndir    *dir_temp;

    current = head;
    temp = current->dir_cont;
    while (current)
    {
        while(temp != NULL)
        {
            if (S_ISDIR(temp->buffer.st_mode))
            {
                dir_temp = new_dir(temp->path);
                //if (current->last && current->last->path == NULL)
                    enqueue_dir(current, dir_temp);
                //else
                //    stack_opndir(current, dir_temp);
            }
            temp = temp->next;
        }
        if (head->next == NULL)
            break;
        head = head->next;
    }
}


void            run_stat_contents(t_cont *head)
{
    t_cont  *current;

    current = head;
    while (current)
    {
        lstat(current->path, &current->buffer);// == -1)
        current = current->next;
    }
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
            //run lstat on everything here path given from argv[y]
            //if its a directory, place on the opndir queue
            //if its a file, inject the name into the add_cont algorithm
            result->dir_cont = add_cont(argv[y], result->dir_cont, flags);
            y++;
        }
    }
    run_stat_contents(result->dir_cont);
    //if there are directories in the list
    build_first_directory_chain(result);
    return (result);
}

t_opndir    *print_full_list(t_opndir *head, int flags)
{
    t_cont  *current;
    t_opndir    *dir;
    (void)flags;
    
    dir = head;
    while (dir)
    {
        printf("Directory: %s\n", dir->path);
        current = dir->dir_cont;    
        while (current != NULL)
        {
            if (S_ISDIR(current->buffer.st_mode) && dir->path == NULL)
                (void)flags;
            else
                printf("\tfile: %s\n", current->path);
            current = current->next;
        }
        if (dir->next == NULL)
            break;
        dir = dir->next;
    }
    return(head->next);
}

void    populate_dir(t_opndir *current, flags)
{
    t_cont  *temp;

    if (current == NULL)
        return;
    current->dir = opendir(current->path);
    struct dirent *readdir(DIR *dirp);
    while ()
    {
//  last took off from her ... need to figure out how to read the contents of a directory
//and place those contents in the t_opndir -> dir_cont linked list
    }
}

int     main(int argc, char **argv)
{
    int         flags;
    t_opndir    *head;

    flags = check_flags(FLAGCHAR, argc, argv);
    //print_flags(flags); //for testing
    head = start_queue(flags, argv);
    while (head != NULL)
    {
        print_full_list(head, flags);
        head = head->next;
        populate_dir(head, flags);
    }
    return(0);
}