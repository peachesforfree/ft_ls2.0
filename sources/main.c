/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalcort <sbalcort@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/09 19:30:38 by sbalcort          #+#    #+#             */
/*   Updated: 2018/07/09 19:30:39 by sbalcort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

char		*new_path(char *prev, char *curr)
{
	char	*temp;

	temp = NULL;
	if (prev[ft_strlen(prev) - 1] != '/')
		temp = ft_strjoin(prev, "/");
	else
		temp = prev;
	temp = ft_strjoin(temp, curr);
	return (temp);
}

void		error_no_option(char c)
{
	ft_printf("ls: illegal option -- ");
	ft_printf("%c\nusage: ls [%s] [file ...]\n", c, FLAGCHAR);
	exit(0);
}

int			hidden_name(char *str)
{
	if (ft_strlen(str) == 1 && str[0] == '.')
		return (1);
	if (ft_strlen(str) == 2 && str[0] == '.' && str[1] == '.')
		return (1);
	return (0);
}

int			not_hidden_dir(t_opndir *head, t_cont *current, int flags)
{
	char	*file;
	int		len;

	if (head->path == NULL)
		return (1);
	file = ft_strstr(current->path, head->path);
	if (file != NULL)
		file = &file[ft_strlen(head->path) + 1];
	len = ft_strlen(file);
	if (len == 1 && file[0] == '.')
		return (0);
	if (len == 2 && file[0] == '.' && file[1] == '.')
		return (0);
	if (len >= 1 && file[0] == '.' && !(flags & HIDFLG))
		return (0);
	return (1);
}

int			main(int argc, char **argv)
{
	int			flags;
	t_opndir	*head;

	flags = flag_checker(argv);
	head = start_queue(flags, argv, argc);
	while (head != NULL)
	{
		print_dir_cont(head, flags);
		if ((flags & RECFLG))
			build_directory_chain(head, flags);
		head = head->next;
	}
	return (0);
}
