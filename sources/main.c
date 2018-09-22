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

int			is_special_dir(char *path);

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
	ft_printf("%c\nusage: ft_ls [%s] [file ...]\n", c, FLAGCHAR);
	return (-1);
}

int			is_directory(char *path)
{
	struct stat		buffer;
	//char			*temp;

	if (lstat(path, &buffer) != 0)
		return (0);
	// temp = ft_strrchr(path, '/') + 1;
	// if (ft_strlen(temp) == 1 && temp[0] == '.')
	// 	return (0);
	return (1);
}

int			is_special_dir(char *path)
{
	char			*temp;
	int				len;

	if (path == NULL)
		return (0);
	if (path[0] == '/')
		return (0);
	if ((temp = ft_strstr(path, "/.")) != NULL)
		len = ft_strlen(temp);
	if (temp != NULL && len == 2 && len != (int)ft_strlen(path))
		return (1);
	if (temp != NULL && len == 1 && ft_strlen(path) > 3 && path[ft_strlen(path) - 1] == '.')
		return (1);
	if ((temp = ft_strstr(path, "/..")) != NULL)
		len = ft_strlen(temp);
	if (temp != NULL && len == 3 && len != (int)ft_strlen(path))
		return (1);
	
	temp = ft_strrchr(path, '/');
	if (temp != NULL && ft_strlen(temp) <= 3 && ft_strlen(path) > 2)
	{
		if (ft_strstr(temp,"/.."))
			return (1);
		if (ft_strstr(temp, "/."))
			return(1);
	}
	return (0);
}

int			main(int argc, char **argv)
{
	int			flags;
	t_opndir	*head;
	
	flags = flag_checker(argv);
	if (flags < 0)
		return (1);
	head = start_queue(flags, argv, argc);
	//print_dir_cont(head, flags);
	//head = head->next;
	while (head != NULL)
	{
		if (!is_special_dir(head->path))
		{
			print_dir_cont(head, flags);
			if ((flags & RECFLG) && head->path != NULL && is_directory(head->path))
				build_directory_chain(head, flags);
		}
		head = head->next;
	}
	return (0);
}