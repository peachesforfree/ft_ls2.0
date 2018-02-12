#include "ls.h"
/*


t_opndir    *opndir_init(void)
{
    t_opndir    *temp;

    temp = opndir_new(NULL, NULL);
    temp->dp = cont_new(NULL, NULL, temp);
    return (temp);
}

//this runs thru the char **dir and assembles a linked list.

t_opndir    *opndir_mk_lst(t_env *env)
{
    t_opndir    *temp;
    int         i;

    i = 0;
    temp = opndir_init();
    while (env->dir[i] != NULL)
    {
        

    }
    return (temp);
}

void    list_contents(t_opndir *s)
{
    t_cont  *temp_cont;

    temp_cont = s->dp;
    while ((temp_cont != NULL) && (temp_cont->cont = readdir(s->dir)) != NULL)
    {
        cont_new(NULL, temp_cont, s);
        temp_cont = temp_cont->next;
    }
    //sort_list(s);
    //queue_new_directories(s);
    print_list(s);
}

void    open_directory(char *path)
{
    t_opndir    *s;

    s = opndir_mk_lst(env);  //change this this to make a chain of all path name
    
    
    //then while thru file names first
    //then while thru directories
    if ((s->dir = opendir(path)) == NULL)
        err(path); //change this to display file name 
    list_contents(s);
    closedir(s->dir);
}
*/
//for printing the queue TESTING
void    print_queue(t_opndir *queue)
{
    t_cont  *cont_current;
    t_opndir *queue_current;
    
    queue_current = queue;
    while (queue_current)
    {
        cont_current = queue_current->dir_cont;
        printf("queue\t%s\n", queue_current->path);
        while (cont_current)
        {
            printf("\t\t%s\n", cont_current->path);
            cont_current = cont_current->next;
        }
        if (queue_current->next)
            queue_current = queue_current->next;
        else
            break;
    }
}

int     err(char *str)
{
    printf("Sorry can not open '%s'", str);
    return (0);
}





















t_cont  *cont_new(char *path, t_cont *next, t_cont *last, struct stat *buffer)
{
    t_cont          *temp;
    
    temp = (t_cont*)malloc(sizeof(t_cont) * 1);
    bzero(temp, sizeof(t_cont));
    if (next != NULL)
    {
        temp->next = next;
        next->last = temp;
    }
    if (last != NULL)
    {
        temp->last = last;
        last->next = temp;
    }
    if (path != NULL)
        temp->path = ft_strdup(path);
    if (buffer != NULL)
        ft_memcpy(&temp->cont, buffer, sizeof(buffer));
    return (temp);
}

t_opndir    *opndir_new(char *path, t_opndir *next, t_opndir *last)
{
    t_opndir    *temp;

    temp = (t_opndir*)malloc(sizeof(t_opndir) * 1);
    bzero(temp, sizeof(t_opndir));
    if (next != NULL)
    {
        temp->next = next;
        next->last = temp;
    }
    if (last != NULL)
    {    
        last->next = temp;
        temp->last = last;
    }
    if (path)
        temp->path = ft_strdup(path);
    return (temp);
}

t_opndir    *insert_dir(char *path, t_opndir *head)
{
    t_opndir    *current;
    
    current = head;
    while (current)
    {
        if (current->path && ft_strcmp(current->path, path) > 0 && ft_strcmp(current->path, path) < 0)
        {
            current->next->last = opndir_new(path, current, current);
            current->next = current->next->last;
        }
        if (current->next == NULL)
        {
            current->next = opndir_new(path, NULL, current);
            return (head);
        }
        current = current->next;
    }
    return (head);
}

/*
**this is to start the linked list/s
*/

void    assemble_contents(t_env *env, t_opndir *head)
{
    (void)env;
    struct stat     buffer;
    char            *str;

    str = NULL;
    if (!head->path)
        return;
    if ((head->dir = opendir(head->path)) == NULL)
        err(head->path);
    while ((head->dirent = readdir(head->dir)) != NULL)
    {
        if (env->a == false)
        {
            if (head->dirent->d_name[0] == '.')
                continue;
        }
        str = ft_strnjoin(head->path, ft_strjoin( "/", head->dirent->d_name), 2);
        if (lstat(str, &buffer) == 0)
            head->dir_cont = insert_cont(head->dirent->d_name, head->dir_cont, &buffer);
        else
            head->dir_cont = insert_cont(head->dirent->d_name, head->dir_cont, NULL);
    }
    if (str != NULL)
        free(str);
    closedir(head->dir);
}


/*
**searches the file queue and what ever is a directory is 
**put into the directory queue.
*/

void            recursive_search(t_env *env, t_opndir *queue)
{
    (void)env;
    t_cont      *cont_temp;
    t_opndir    *dir_temp;
    char        *temp_path;

    cont_temp = queue->dir_cont;
    // while (cont_temp && cont_temp->next != NULL)
    //     cont_temp = cont_temp->next;
    while(cont_temp) //&& cont_temp->next != NULL)
    {
        if (S_ISDIR(cont_temp->cont.st_mode) && ft_strcmp(cont_temp->path, ".") != 0)
        {
            temp_path = ft_strjoin(queue->path, "/");
            temp_path = ft_strnjoin(temp_path, cont_temp->path, 1);
            dir_temp = opndir_new(temp_path, queue->next, queue);
            free(temp_path);
        }
        cont_temp = cont_temp->next;
    }
}


//WORK ON THIS LAST
/*t_opndir    *free_current_head(t_opndir *queue)
{
    t_opndir    *dir_tmp;
    t_cont      *cont_tmp;
    
    dir_tmp = queue;
    cont_tmp = dir_tmp->dir_cont;
    while (cont_tmp)
    {
        
        cont_tmp = cont_tmp->next;
    }
    return (dir_tmp);
}
*/

void    print_contents(t_env *env, t_opndir *head)
{
    t_cont  *temp;
    struct stat tmp;
    
    temp = head->dir_cont;
    if (head->path && ft_strcmp(head->path, "\0") != 0)
    {
        lstat(head->path, &tmp);
        write(1, head->path, ft_strlen(head->path));
        write(1, ":\n", 2);
    }
    if (!temp)
        return ;
    if (env->r == true)
    {
        while (temp)
        {
            write(1, temp->path, ft_strlen(temp->path));
            write(1, "\n", 1);
            //printf("%s\n", temp->path);
            temp = temp->next;
        }
    }
    else
    {
        while (temp->next != NULL)
            temp = temp->next;
        while (temp)
        {
            write(1, temp->path, ft_strlen(temp->path));
            write(1, "\n", 1);
            //printf("%s\n", temp->path);
            temp = temp->last;
        }
    }
}

int     main(int argc, char **argv)
{
    t_env       env;
    t_opndir    *queue;
    t_opndir    *head;  //for testing

    parse_arg(&env, argc, argv);
    queue = assemble_first_queue(&env); //this pulls the env->dir makes a queue for directories/files to be viewed
    head = queue;       //for testing
    while (queue)
    {
        if (env.t == true)                      //sort by time
            time_sort(&env, queue);
        if (env.t == false)                     //sort by alpha
            assemble_contents(&env, queue);
        if (env.R == true)                      //recursive search
            recursive_search(&env, queue);
//        if (env.l == true)                      //to assemble long listing
//            assemble_long_list(&env, queue);
        print_contents(&env, queue);          //for printing and also do -l flag
        //queue = free_current_head(queue);   //do this last freeing shit
        queue = queue->next;
    }
    //print_queue(head);  //also for testing
    return (0);
}

/*
    -a  show hidden files
        this is handled uponreading of files in dir
        DONE

    -R  recursive display flag
        DONE
        
    -t  displays file by last time modified
        handle upon sort/insert (curently only doing by ascii)
        (make sure to order from oldest to newest)
        IN PROGRESS WITHIN TIME_SORT
        
    -l  Long listing format
        handle this upon printing
    
    -r  Prints stuff in reverse order
        need to handle this in initial sorting
        (already sorted from last to first order)
    
    -1  newline between printing items
        include this in printing


    TO BE FIXED
        need to work on file parsing ... error for "../../../" "/dev"
        account for -- edgecase for not considering -- for a flag
        also make sure to do a check for permissions for recursive check.
*/