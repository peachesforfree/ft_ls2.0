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
	int			len;

	if (master->format == NULL)
		master->format = (t_format*)ft_memalloc(sizeof(t_format));
//	master->format->digit_count_hard = 0;
//	master->format->digit_count_size = 0;
//	master->format->len_owner = 0;
//	master->format->len_group = 0;
	temp = master->dir_cont;
	while (temp && !lstat(temp->path, &buffer) )
	{
		//dprintf(2, "st_nlink:%d\nst_size:%lld\n",buffer.st_nlink, buffer.st_size);
		if (master->format->digit_count_hard < buffer.st_nlink)
			master->format->digit_count_hard = buffer.st_nlink;
		if (master->format->digit_count_size < buffer.st_size)
			master->format->digit_count_size = buffer.st_size;
		len = ft_strlen(getpwuid(buffer.st_uid)->pw_name);
		if (master->format->len_owner < len)
			master->format->len_owner = len;
		len = ft_strlen(getgrgid(buffer.st_gid)->gr_name);
		if (master->format->len_group < len)
			master->format->len_group = len;
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
}

int			new_line(t_opndir *master)
{
	if (master->last != NULL && master->last->path == NULL && master->last->dir_cont != NULL)
		ft_printf("\n");
	else if (master->last != NULL && master->last->path != NULL)
	 	ft_printf("\n");
	// if (master->last && master->last->last != NULL && master->last->path != NULL)
	// {
	// 	ft_printf("\n");

	// }
	// else if (master->last && master->last->path == NULL && master->last->dir_cont != NULL)
	// {	
	// 	ft_printf("\n");
	// }
	if(master->next == NULL && master->last != NULL && master->last->path == NULL && master->last->dir_cont == NULL && master->last->flgs != 1)
		return (0);
		//return (1);
	//		ft_printf("\n");
	return (1);
}

void		print_dir_cont(t_opndir *current, int flags)
{
	t_cont		*temp;

	
	//if (current->format == NULL)
	get_format_stats(current);
	temp = current->dir_cont;

	t_cont *derp = current->dir_cont;
	if (derp)
	{
		while(derp->last != NULL)
			derp = derp->last;
		while (derp != NULL)
		{
			//dprintf(2, "\t2\tcont_path:%s\n", derp->path);
			derp = derp->next;
		}
	}
	
	// t_cont *printer = current->dir_cont;
	// if (printer)
	// {
	// 	while(printer->last != NULL)
	// 		printer = printer->last;
	// 	while (printer != NULL)
	// 	{
	// 		dprintf(2, "\tcont_path:%s\t", printer->path);
	// 		printer = printer->next;
	// 	}
	// 		dprintf(2, "\n\n");

	// }
	if (current->last != NULL && new_line(current))//multiple_dir(current) && current->path != NULL && new_line(current))//flags & RECFLG || flags & LONGFLG || //&& temp != NULL)
		ft_printf("%s:\n", current->path);
	//dprintf(2, "%s\t%d\t%d\n",current->path, multiple_dir(current), new_line(current));
	
	if (directory_permission_check(current))
		return ;
	if ((flags & LONGFLG ||
		multiple_dir(current)) && temp != NULL) //flags & RECFLG || 
		if (current->path != NULL && flags & LONGFLG)
			ft_printf("total %d\n", print_blocks(current, flags));
	if (flags & REVFLG)
		while (temp && temp->next != NULL)
			temp = temp->next;
	//dprintf(2, "TEST\topndir:%s\n", current->path);
	while (temp != NULL)
	{
		cont(temp, flags, current);
		temp = iterate_t_cont(temp, flags);
	}
}
