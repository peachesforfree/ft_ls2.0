#include "../includes/ft_ls.h"

t_cont		*go_to_end(t_cont *temp)
{
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

int			multiple_dir(t_opndir *head)
{
	
	if (head->next != NULL)
		return (1);
	if (head->last != NULL)
		return (1);
	return (0);
}

t_cont		*iterate(t_cont *current, int flags)
{
	if (flags & REVFLG)
		return (current->next);
	else
		return (current->last);
}

t_cont      *sex_and_magic(t_opndir *head, t_cont *current, char *path)
{
	if (head->path != NULL || !head || !path)
		return (NULL);
	while (current)
	{
		if (!ft_strcmp(current->path, path))
			return (current);
		current = current->next;
	}
	return (current);
}
