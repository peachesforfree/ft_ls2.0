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

#include "ft_ls.h"

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
	DIR		*dirent;

	if (current->path == NULL)
		return (0);
	errno = 0;
	dirent = opendir(current->path);
	if (errno != 0)
	{
		ft_printf("\n%s:\nft_ls: %s: %s\n", current->path,
		current->path, strerror(errno));
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
		ft_printf("ft_ls: %s: No such file or directory\n", str);
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

/*
**To assemble first queue. Either assembles the list of stated items
** OR makes current directory the only item in head list
*/

t_opndir	*start_queue(int flags, char **argv, int argc)
{
	t_opndir	*result;
	int			y;

	y = 1;
	result = (t_opndir*)ft_memalloc(sizeof(t_opndir));
	while (argv[y] != NULL && argv[y][0] == '-')
		y++;
	if (argc == y)
		result->dir_cont = new_cont(".", NULL, NULL);
	else
	{
		while (argv[y] != NULL)
		{
			if (does_exist(argv[y]))
				result->dir_cont = add_cont(argv[y], result->dir_cont, flags);
			y++;
		}
	}
	run_stat_contents(result->dir_cont);
	build_directory_chain(result, flags);
	remove_directories(result);
	return (result);
}
