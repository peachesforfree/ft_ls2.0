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

int			error_no_option(char c)
{
	ft_printf("ls: illegal option -- ");
	ft_printf("%c\nusage: ls [%s] [file ...]\n", c, FLAGCHAR);
	return (-1);
}

int			is_directory(char *path)
{
	struct stat		buffer;

	if (lstat(path, &buffer) != 0)
		return (0);
	if (S_ISDIR(buffer.st_mode))
	{
		if (ft_strstr(path, "/.") != NULL)
			return (0);
		if (ft_strstr(path, "/..") != NULL)
			return (0);
	}
	return (1);
}

int			main(int argc, char **argv)
{
	int			flags;
	t_opndir	*head;
	
	flags = flag_checker(argv);
	if (flags < 0)
		return (1);
	head = start_queue(flags, argv, argc);
	while (head != NULL)
	{
		print_dir_cont(head, flags);
		if ((flags & RECFLG) && head->path != NULL && is_directory(head->path))
			build_directory_chain(head, flags);
		head = head->next;
	}
	return (0);
}