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

void		cont(t_cont *temp, int flags, t_opndir *master)
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
		long_format_print(temp, master->format);
	if (name)
		ft_putstr(name);
	if (S_ISLNK(temp->buffer.st_mode) && (flags & LONGFLG))
		link_name(temp);
	if (name)
		ft_putchar('\n');
}

void		get_format_stats(t_opndir *master)
{
	t_cont		*temp;
	int			i;
	struct stat	buffer;

	if (master->format == NULL)
		master->format = (t_format*)ft_memalloc(sizeof(t_format));
	master->format->digit_count_hard = 0;
	master->format->digit_count_size = 0;
	temp = master->dir_cont;
	while (temp && !lstat(temp->path, &buffer))
	{
		//dprintf(2, "st_nlink:%d\nst_size:%lld\n",buffer.st_nlink, buffer.st_size);
		if (master->format->digit_count_hard < buffer.st_nlink)
			master->format->digit_count_hard = buffer.st_nlink;
		if (master->format->digit_count_size < buffer.st_size)
			master->format->digit_count_size = buffer.st_size;
		temp = temp->next;
	}
	i = 1;
	while(master->format->digit_count_hard >= 10)
	{
		master->format->digit_count_hard /= 10;
		i++;
	}
	master->format->digit_count_hard = i;
	i = 1;
	while (master->format->digit_count_size >= 10)
	{
		master->format->digit_count_size /= 10;
		i++;
	}
	master->format->digit_count_size = i;
	//dprintf(2, "Hard link digit count:%d\nfile byte size digit count:%d\n", master->format->digit_count_hard,master->format->digit_count_size);
}

int			new_line(t_opndir *master)
{
	if (master->last->last != NULL && master->last->path != NULL)
		ft_printf("\n");
	else if (master->last->path == NULL && master->last->dir_cont != NULL)
		ft_printf("\n");
	if (master->last->last == NULL)
	{
		if(master->next == NULL)
			return (0);
		return (1);
	}
	return (1);
}

void		print_dir_cont(t_opndir *current, int flags)
{
	t_cont		*temp;

	if (current->format == NULL)
		get_format_stats(current);
	temp = current->dir_cont;
	if ((flags & RECFLG || flags & LONGFLG ||
		multiple_dir(current))) //&& temp != NULL)
		if (current->path != NULL && new_line(current))
			ft_printf("%s:\n", current->path);
	if (directory_permission_check(current))
		return ;
	if ((flags & LONGFLG ||
		multiple_dir(current)) && temp != NULL) //flags & RECFLG || 
		if (current->path != NULL && flags & LONGFLG)
			ft_printf("total %d\n", print_blocks(current, flags));
	if (flags & REVFLG)
		while (temp && temp->next != NULL)
			temp = temp->next;
	while (temp != NULL)
	{
		cont(temp, flags, current);
		temp = iterate_t_cont(temp, flags);
	}
}
