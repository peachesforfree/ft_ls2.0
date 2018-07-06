/*********************************************
 * note: something is wrong with time sort.
 *          fix after long printing is done.
 * 
 * **********************************************/

#include "ft_ls.h"

void    error_no_option(char c)
{
    printf("ls: illegal option -- %c\nusage: ls [%s] [file ...]", c, FLAGCHAR);
    exit(0);
}

int     flag_checker(char *flags, int argc, char **argv)
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

    lstat(path, &temp->buffer);

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

t_cont      *insert_time(char *path, t_cont *head)
{
    t_cont  *temp;
    t_cont  *current;

    current = head;
    temp = new_cont(path, NULL, NULL);
    lstat(temp->path, &temp->buffer);
    if (current == NULL)
        return(temp);   //maybe move this to bottom of function?
    while (current != NULL)
    {
        if (current->buffer.st_mtimespec.tv_sec < temp->buffer.st_mtimespec.tv_sec
        || ((current->buffer.st_mtimespec.tv_sec == temp->buffer.st_mtimespec.tv_sec)
        && (current->buffer.st_mtimespec.tv_nsec < temp->buffer.st_mtimespec.tv_nsec)))
    //    && current->buffer.st_mtimespec.tv_nsec < temp->buffer.st_mtimespec.tv_nsec)  //compares last modification down to the nanosecond
        {
            if (current->last == NULL)
            {
                temp->next = current;
                current->last = temp;
                return (temp);
            }
            else if (current->last != NULL)
            {
                temp->last = current->last;
                temp->next = current;
                temp->last->next = temp;
                temp->next->last = temp;
                return (head);
            }
        }
        if (current->next == NULL)
        {
            temp->last = current;
            current->next = temp;
            return(head);
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
    if (flags & TIMFLG)
        return(insert_time(path, head));
    else
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


void            build_first_directory_chain(t_opndir *head, int flags)
{
    t_cont      *temp;
    t_opndir    *current;
    t_opndir    *dir_temp;

    current = head;
    temp = current->dir_cont;
    while (head != NULL)
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
               populate_dir(dir_temp, flags);
            }
            temp = temp->next;
        }
        head = head->next;
    }
}

char            *new_path(char *prev, char *curr)
{
    char *temp;

    temp = ft_strjoin(prev, "/");
    temp = ft_strnjoin(temp, curr, 1);
    return (temp);
}

t_cont      *go_to_end(t_cont *temp)
{
    while (temp->next != NULL)
        temp = temp->next;
    return (temp);
}

/*****************************
 * 
 *  some issue here with actually going through opndir->dir_cont list 
 *  need to find out how to figure out if item in list is a directory 
 *  and how to pop it onto the opndir stack
 * 
 * ********************************/

void            build_directory_chain(t_opndir *head, int flags)
{
    t_cont      *temp;
    t_opndir    *current;
    t_opndir    *dir_temp;

    current = head;
    //while (current != NULL)
    //{
        temp = go_to_end(current->dir_cont);
        while(temp != NULL)
        {
            if ((ft_strcmp(temp->path, ".") && ft_strcmp(temp->path, "..")) && S_ISDIR(temp->buffer.st_mode))
            {
                dir_temp = new_dir(new_path(current->path, temp->path));
                stack_opndir(current, dir_temp);
                populate_dir(dir_temp, flags);
            }
            temp = temp->last;
        }
    //    current = current->next;
    //}
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
    build_first_directory_chain(result, flags);
    return (result);
}

void    populate_dir(t_opndir *current, int flags)
{

    if (current == NULL)
        return;
    current->dir = opendir(current->path);
    struct dirent *readdir(DIR *dirp);
    while ((current->dirent = readdir(current->dir)) != NULL)
        current->dir_cont = add_cont(current->dirent->d_name, current->dir_cont, flags);
    closedir(current->dir);
}

t_cont    *iterate_t_cont(t_cont *temp, int flags)
{
    if (flags & REVFLG)
        return (temp->last);
    else
        return (temp->next);

}

void    long_format_print(t_cont *current)
{
    lstat(current->path, &current->buffer);
    //printing file mode 
    if (S_ISREG(current->buffer.st_mode))
        ft_putchar('-');
    else if (S_ISDIR(current->buffer.st_mode))
        ft_putchar('d');
    else if (S_ISLNK(current->buffer.st_mode))
        ft_putchar('l');
    //printing owner permissions
    ft_putchar((S_IRUSR & current->buffer.st_mode) ? 'r' : '-'); 
    ft_putchar((S_IWUSR & current->buffer.st_mode) ? 'w' :'-');
    ft_putchar((S_IXUSR & current->buffer.st_mode) ? 'x' : '-');
    //peinting group permissions
    ft_putchar((S_IRGRP & current->buffer.st_mode) ? 'r' : '-'); 
    ft_putchar((S_IWGRP & current->buffer.st_mode) ? 'w' :'-');
    ft_putchar((S_IXGRP & current->buffer.st_mode) ? 'x' : '-');
    //others permissions
    ft_putchar((S_IROTH & current->buffer.st_mode) ? 'r' : '-'); 
    ft_putchar((S_IWOTH & current->buffer.st_mode) ? 'w' :'-');
    ft_putchar((S_IXOTH & current->buffer.st_mode) ? 'x' : '-');
    //number of hard links
    ft_printf("%3d", current->buffer.st_nlink);
    //user
    ft_printf(" %s ", getpwuid(current->buffer.st_uid)->pw_name);
    //group
    ft_printf(" %s", getgrgid(current->buffer.st_gid)->gr_name);
    //size in bytes
    ft_printf("%8lld ", current->buffer.st_size);
    //date
    //printf("%s %d ", ctime(&current->buffer.st_mtim.tm_mon), current->buffer.st_mtim.tm_mday);
    char    *time_str;

    time_str = &ctime(&current->buffer.st_mtimespec.tv_sec)[4];
    ft_bzero(&time_str[12], 1);
    ft_printf ("%s ", time_str);
    //printf("%5 ", hour, minute) OR printf("%5d ", year)
    }

void    print_dir_cont(t_opndir *current, int flags)
{
    t_cont  *temp;

    temp = current->dir_cont;
    if (flags & REVFLG)
    {
        while (temp->next != NULL)
            temp = temp->next;
    }
    if (flags & RECFLG)
    {
        if (current->path != NULL)
            ft_printf("%s:\n", current->path);
    }
    while(temp != NULL)
    {
        if (!(flags & HIDFLG) && temp->path && temp->path[0] == '.')
        {
            temp = iterate_t_cont(temp, flags);
            continue;
        }
        if (flags & LONGFLG)
            long_format_print(temp);
        ft_printf("%s\n", temp->path);
        //need to insert here a way to print where to symbolic link leads to THEN a new line
        temp = iterate_t_cont(temp, flags);
    }
}

int     main(int argc, char **argv)
{
    int         flags;
    t_opndir    *head;

    flags = flag_checker(FLAGCHAR, argc, argv);
    //print_flags(flags); /**************************  for testing           */
    head = start_queue(flags, argv);
    while (head != NULL)
    {
        print_dir_cont(head, flags);
        head = head->next;
        if (flags & RECFLG && head != NULL)
            build_directory_chain(head, flags);
    }
    return(0);
}