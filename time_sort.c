# include <ls.h>

t_cont    *time_sort(t_env *env, t_cont *head)
{
    while (head->next)
    {
        
    }
    //append to end
    return (head);
}


t_opndir    *time_assemble_sort(t_env *env)
{
    t_opndir    *head;
    int         i;

    head = opndir_new("\0", NULL, NULL);
    while (env->dir[i])
    {
        if (head->dir_cont == NULL)
            head->dir_cont = opndir_new(env->dir[i], NULL, NULL);
        else 
            head->dir_cont = time_sort(env, head->dir_cont); 
        i++;
    }
    //init nnew t_opndr struct NULL NULL NULL
    //lstat all files in DIR
        //if directory place a new one in queue (by time created)
        //if file, sort into NULL NULL NULL     (by time created)
    return (head);
}