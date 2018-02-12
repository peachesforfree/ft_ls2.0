
#include "ls.h"

void        reverse_sort(t_opndir *s)
{
    (void)s;

}

void        sort_list(t_opndir *s)
{
    t_cont  *tmp;
    t_cont  *curr;
    (void)tmp;
    curr = s->dp;
    if (s->r == 1)
    {
        reverse_sort(s);
        return ;
    }
    while (curr->next != NULL)
    {
        

    }

}
