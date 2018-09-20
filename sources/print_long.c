/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_long.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalcort <sbalcort@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/09 19:31:14 by sbalcort          #+#    #+#             */
/*   Updated: 2018/07/09 19:31:16 by sbalcort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int			print_blocks(t_opndir *head, int flags)
{
	int		count;
	t_cont	*temp;

	(void)flags;
	count = 0;
	temp = head->dir_cont;
	while (temp != NULL)
	{
		lstat(temp->path, &temp->buffer);
		count += temp->buffer.st_blocks;
		temp = temp->next;
	}
	return (count);
}

void		continued(t_cont *current)
{
	char	*time_str;

	time_str = &ctime(&current->buffer.st_mtimespec.tv_sec)[4];
	ft_bzero(&time_str[12], 1);
	ft_putstr(time_str);
}

char		*format_ls_str(int number, char *c)
{
	char	*ret;
	char	*ret_number;

	ret_number = ft_itoa(number + 1);

	ret = ft_strjoin(" % ", ret_number);
	ret = ft_strjoin(ret, c);
	return (ret);
}

void		leading_bit(struct stat *buffer)
{
	if (S_ISDIR(buffer->st_mode))
		ft_putchar('d');
	else if (S_ISLNK(buffer->st_mode))
		ft_putchar('l');
	else if (S_ISBLK(buffer->st_mode))
		ft_putchar('b');
	else if (S_ISCHR(buffer->st_mode))
		ft_putchar('c');
	else if (S_ISSOCK(buffer->st_mode))
		ft_putchar('s');
	else if (S_ISFIFO(buffer->st_mode))
		ft_putchar('p');
	else
		ft_putchar('-');
}

void		last_bit(struct stat *buffer)
{
	if (S_IROTH & buffer->st_mode)
		ft_putchar('r');
	else
		ft_putchar('-');
	if (S_IWOTH & buffer->st_mode)
		ft_putchar('w');
	else
		ft_putchar('-');
	if (S_IXOTH & buffer->st_mode)
		ft_putchar('x');
	else if (S_ISVTX & buffer->st_mode && (!(S_IXOTH & buffer->st_mode) || !(S_IWOTH & buffer->st_mode)))
		ft_putchar('T');
	else if (S_ISVTX & buffer->st_mode && (S_IXOTH & buffer->st_mode || S_IWOTH & buffer->st_mode))
		ft_putchar('t');
	else
		ft_putchar('-');
}

char		*format_name_str(int len, char *str)
{
	char *ret;

	ret = ft_strnjoin("%- ", ft_itoa(len), 2);
	ret = ft_strnjoin(ret, str, 1);
	return (ret);
}

void		long_format_print(t_cont *current, t_format *format)
{
	lstat(current->path, &current->buffer);
	leading_bit(&current->buffer);
	ft_putchar((S_IRUSR & current->buffer.st_mode) ? 'r' : '-');
	ft_putchar((S_IWUSR & current->buffer.st_mode) ? 'w' : '-');
	ft_putchar((S_IXUSR & current->buffer.st_mode) ? 'x' : '-');
	ft_putchar((S_IRGRP & current->buffer.st_mode) ? 'r' : '-');
	ft_putchar((S_IWGRP & current->buffer.st_mode) ? 'w' : '-');
	ft_putchar((S_IXGRP & current->buffer.st_mode) ? 'x' : '-');
	last_bit(&current->buffer);
	ft_printf(format_ls_str(format->digit_count_hard,"d "), current->buffer.st_nlink);
	ft_printf(format_name_str(format->len_owner, "s  "), getpwuid(current->buffer.st_uid)->pw_name);
	ft_printf(format_name_str(format->len_group, "s"), getgrgid(current->buffer.st_gid)->gr_name);
	ft_printf(format_ls_str(format->digit_count_size,"d "), current->buffer.st_size);
	continued(current);
	ft_putchar(' ');
}
