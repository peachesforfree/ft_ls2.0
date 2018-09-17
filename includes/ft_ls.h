/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalcort <sbalcort@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/09 22:49:40 by sbalcort          #+#    #+#             */
/*   Updated: 2018/07/09 22:49:41 by sbalcort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>
# include <strings.h>
# include <stdbool.h>
# include "../libft/libft.h"
# include <pwd.h>
# include <grp.h>
# include <time.h>
# include "../ft_printf/ft_printf.h"
# include <errno.h>

#include <stdio.h>

# define FLAGCHAR "-lRart1"
# define LONGFLG    0b00000001
# define RECFLG     0b00000010
# define HIDFLG     0b00000100
# define REVFLG     0b00001000
# define TIMFLG     0b00010000
# define DIRCNT		0b00100000
# define NOOPTION   1

typedef struct		s_format
{
	int				digit_count_hard;
	int				digit_count_size;
	int				len_owner;
	int				len_group;
}					t_format;

typedef struct		s_cont
{
	char			*path;
	struct stat		buffer;
	struct s_cont	*next;
	struct s_cont	*last;
}					t_cont;

typedef struct		s_opndir
{
	char			*path;
	DIR				*dir;
	struct dirent	*dirent;
	struct s_cont	*dir_cont;
	struct s_opndir	*next;
	struct s_opndir	*last;
	struct s_format	*format;
	int				flgs;
}					t_opndir;

// void				populate_dir(t_opndir *current, int flags);
// char				*new_path(char *prev, char *curr);
// int					not_hidden_dir(t_opndir *head, t_cont *current, int flags);
// int					directory_permission_check(t_opndir *current);
int					flag_checker(char **argv, int *i);
// int					error_no_option(char c);
// t_cont				*new_cont(char *path, t_cont *before, t_cont *after);
// t_cont				*add_cont(char *path, t_cont *head, int flags);
// t_opndir			*new_dir(char *path);
// void				stack_opndir(t_opndir *current, t_opndir *new);
// void				enqueue_dir(t_opndir *head, t_opndir *new);
// char				*new_path(char *prev, char *curr);
// t_cont				*insert_time(char *path, t_cont *head);
// t_cont				*insert_alpha(char *path, t_cont *head);
// void				long_format_print(t_cont *current, t_format *format);
// int					print_blocks(t_opndir *head, int flags);
// t_cont				*iterate_t_cont(t_cont *temp, int flags);
// void				print_dir_cont(t_opndir *current, int flags);
// int					multiple_dir(t_opndir *head);
// void				run_stat_contents(t_cont *head);
// void				build_directory_chain(t_opndir *head, int flags);
// t_opndir			*start_queue(int flags, char **argv, int argc);
// void				populate_dir(t_opndir *current, int flags);
// int					directory_permission_check(t_opndir *current);
// void				remove_directories(t_opndir *head);



void		print_full_chain(t_opndir *head);

#endif
