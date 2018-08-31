/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalcort <sbalcort@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/09 19:30:57 by sbalcort          #+#    #+#             */
/*   Updated: 2018/07/09 19:30:58 by sbalcort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

t_cont		*iterate_t_cont(t_cont *temp, int flags)
{
	if (flags & REVFLG)
		return (temp->last);
	else
		return (temp->next);
}

void		link_name(t_cont *temp)
{
	char	*sylink;
	int		count;

	count = 0;
	sylink = ft_strnew(1024);
	readlink(temp->path, sylink, 1024);
	ft_printf(" -> %s", sylink);
}

void		cont(t_cont *temp, int flags)
{
	char	*name;

	name = temp->path;
	if ((name = ft_strrchr(name, '/')) != NULL)
		name += 1;
	else
		name = temp->path;
	if (!(flags & HIDFLG) && name && (name[0] == '.'))
		name = NULL;
	if (name != NULL && (flags & LONGFLG))
		long_format_print(temp);
	if (name)
		ft_putstr(name);
	if (S_ISLNK(temp->buffer.st_mode) && (flags & LONGFLG))
		link_name(temp);
	if (name)
		ft_putchar('\n');
}

t_cont	*find_start(t_cont *current)
{
	while (current && current->last != NULL)
		current = current->last;
	return (current);
}

void		print_dir_cont(t_opndir *current, int flags)
{
	t_cont		*temp;

	temp = find_start(current->dir_cont);
	if ((flags & RECFLG || flags & LONGFLG ||
		multiple_dir(current)) && temp != NULL)
		if (current->path != NULL)
			ft_printf("\n%s:\n", current->path);
	if (directory_permission_check(current))
		return ;
	if ((flags & RECFLG || flags & LONGFLG ||
		multiple_dir(current)) && temp != NULL)
		if (current->path != NULL)
			ft_printf("total %d\n", print_blocks(current, flags));
	if (flags & REVFLG)
		while (temp && temp->next != NULL)
			temp = temp->next;
	while (temp != NULL)
	{
		cont(temp, flags);
		temp = iterate_t_cont(temp, flags);
	}
}
