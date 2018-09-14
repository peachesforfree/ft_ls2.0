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

/*
	problem with printing results

	need to look into the stat buffer in void		get_format_stats(t_opndir *master) in print_files.c
	wrong value from struct is being used and need to find the right one.
	its screwing up values in the moulitest

*/

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

int			not_hidden_dir(t_opndir *head, t_cont *current, int flags)
{
	char	*file;
	int		len;

	//dprintf(2, "head->path:%s\tcurrent->path:%s\n",head->path, current->path);
	if (head->path == NULL)
		return (1);
	file = ft_strstr(current->path, head->path);
	if (file != NULL)
		file = &file[ft_strlen(head->path) + 1];
	len = ft_strlen(file);
	if (len == 1 && file[0] == '.')
		return (0);
	if (len == 2 && file[0] == '.' && (file[1] == '.' || file[1] == '/'))
		return (0);
	if (len == 3 && file[0] == '.' && file[1] == '.' && file[2] == '/' )
		return (0);
	if (len >= 1 && file[0] == '.' && !(flags & HIDFLG))
		return (0);
	return (1);
}

int			main(int argc, char **argv)
{
	int			flags;
	t_opndir	*head;
	t_opndir	*temp;

	// int i;
	// for (i = 0; i < argc; i = i + 1)
	// 	dprintf(2, "\tUSER IN:%s\n", argv[i]);

	
	flags = flag_checker(argv);
	if (flags < 0)
		return (1);
	head = start_queue(flags, argv, argc);
	temp = head;
	while (temp != NULL)
	{
		print_dir_cont(temp, flags);
		if ((flags & RECFLG))
			build_directory_chain(temp, flags);
		temp = temp->next;
	}
	if (head->flgs == 1)
		return(1);
//	free_all();
	return (0);
}