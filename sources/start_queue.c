/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_queue.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalcort <sbalcort@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/09 19:31:40 by sbalcort          #+#    #+#             */
/*   Updated: 2018/07/09 19:31:41 by sbalcort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int			does_exist(char *str)
{
	struct stat		buffer;

	if (lstat(str, &buffer) < 0)
	{
		ft_printf("ls: %s: No such file or directory\n", str);
		return (0);
	}
	return (1);
}

void		populate_dir(t_opndir *current, int flags)
{
	char	*new;

	if (current == NULL)
		return ;
	current->dir = opendir(current->path);
	if (current->dir == NULL)
		return ;
	while ((current->dirent = readdir(current->dir)) != NULL)
	{
		if (!(flags & HIDFLG) && (current->dirent->d_name[0] == '.'))
			continue;
		new = new_path(current->path, current->dirent->d_name);
		if (new != NULL)
			current->dir_cont = add_cont(new, current->dir_cont, flags);
	}
	closedir(current->dir);
}

void		print_full_chain(t_opndir *head)
{
	t_opndir	*temp;
	t_cont		*files;

	temp = head;
	while (temp != NULL)
	{
		files = temp->dir_cont;
		dprintf(2, "\t\tPATH:%s\n", temp->path);
		while (files != NULL)
		{
			dprintf(2, "\t\t\t\tCONT:%s\n", files->path);
			files = files->next;
		}
		temp = temp->next;
	}
}

int			set_y(char **argv)
{
	int y;

	y = 1;
	while (argv[y] != NULL && argv[y][0] == '-' && argv[y][1] != '\0')
		y++;
	return (y);
}

t_opndir	*start_queue(int flags, char **argv, int argc)
{
	t_opndir	*result;
	int			y;

	y = set_y(argv);
	result = (t_opndir*)ft_memalloc(sizeof(t_opndir));
	if (y == argc)
	{
		result->path = ft_strdup(".");
		populate_dir(result, flags);
	}
	else
	{
		while (argv[y] != NULL)
		{
			if (does_exist(argv[y]))
				result->dir_cont = add_cont(argv[y], result->dir_cont, flags);
			else
				result->flgs = 1;
			y++;
		}
		build_directory_chain(result, flags);
	}
	remove_directories(result);
	return (result);
}
