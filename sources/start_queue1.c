#include "../includes/ft_ls.h"

void		remove_directories(t_opndir *head)
{
	t_cont	*current;
	t_cont	*before;
	t_cont	*after;

	current = head->dir_cont;
	while (current != NULL && S_ISDIR(current->buffer.st_mode))
	{
		if (S_ISDIR(current->buffer.st_mode))
		{
			before = current->last;
			after = current->next;
			if (before != NULL)
				before->next = after;
			if (after != NULL)
				after->last = before;
		}
		if (before != NULL)
			head->dir_cont = before;
		else if (before == NULL && after != NULL)
			head->dir_cont = after;
		else if (before == NULL && after == NULL)
			head->dir_cont = NULL;
		current = after;
	}
}

int			directory_permission_check(t_opndir *current)
{
	DIR			*dirent;

	if (current->path == NULL)
		return (0);
	errno = 0;
	dirent = opendir(current->path);
	if (errno != 0)
	{
		return (1);
	}
	closedir(dirent);
	return (0);
}