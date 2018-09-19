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

t_list	*open_directory(char *directory, int flags);
int		directory_file_read(t_list *head, int flags);

/*
 * bridges front to current and current to last
*/

void	insert_elem(t_list *front, t_list *last, t_list *current)
{
	if (current == NULL)
		return;
	front->next = current;
	current->next = last;
}

/*
 *sort by ascii chars
 * if s2 is less thant s1 return 1 (goes after)
 * else return -1 (goes before)
*/

int		insert_alpha(char *s1, char *s2)
{
	if (s1 == NULL || s2 == NULL)
		return (0);
	if (ft_strcmp(s1, s2) < 0)
		return (-1);
	else
		return (1);
}

int		insert_time(char *s1, char *s2)
{
	struct stat	buffer1;
	struct stat	buffer2;
	
	if (lstat(s1, &buffer1) == 0 && lstat(s2, &buffer2) == 0)
	{
		if (buffer1.st_mtimespec.tv_sec < buffer2.st_mtimespec.tv_sec || ((buffer1.st_mtimespec.tv_sec == buffer2.st_mtimespec.tv_sec) && (buffer1.st_mtimespec.tv_nsec < buffer2.st_mtimespec.tv_nsec)))
			return (1);
	}
	return (0);
}

/*
 *This will roll through each list and apply the sorting function to current item
 * and the new item in question
 * Designed to always be inserting new item after the current list item
 * 		done to keep down time spent iterating through lists
*/

t_list	*append(t_list *head, void *elem, int size, int(*sort)(char*,char*))
{
	t_list	*new_elem;
	t_list	*temp;
	
	new_elem = ft_lstnew(elem, size + 1);
	temp = head;
	if (head == NULL)
		return (new_elem);
	while (head != NULL)
	{
		if (sort(head->content, new_elem->content) < 0) //new elem goes after current
		{
			if (head->next != NULL && sort(head->next->content, new_elem->content) > 0) //next exists and new elem goes before next item in list 
			{
				insert_elem(head, head->next, new_elem);//insert between temp and temp->next
				break ;
			}
			if (head->next == NULL)
			{
				insert_elem(head, NULL, new_elem);//inset at end of list
				break ;
			}
		}
		if (sort(head->content, new_elem->content) > 0) //new_elem goes before head
		{		//maybe make this ft_lstadd()
			new_elem->next = head;
			return (new_elem);
		}
		head = head->next;
	}
	return (temp);
}

/*
 *This will choose the sorting algorithm to be applied
 * by assesing the flags
*/
t_list	*sorted_list(char *str, int flags, t_list *head)
{
	if (flags | REVFLG)
		head = append(head, str, ft_strlen(str), &insert_alpha);			//insert alpha
	else
		head = append(head, str, ft_strlen(str), &insert_time);			//insert time
	return (head);
}

t_list	*start_list(int argc, char **argv, int i, int flags)
{
	t_list	*head;

	head = NULL;
	while (i < argc)
	{	
		head = sorted_list(argv[i], flags, head);
		i++;
	}
	return (head);
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

void	print_file_permissions(struct stat *buffer)
{
	leading_bit(buffer);
	ft_putchar((S_IRUSR & buffer->st_mode) ? 'r' : '-');
	ft_putchar((S_IWUSR & buffer->st_mode) ? 'w' : '-');
	ft_putchar((S_IXUSR & buffer->st_mode) ? 'x' : '-');
	ft_putchar((S_IRGRP & buffer->st_mode) ? 'r' : '-');
	ft_putchar((S_IWGRP & buffer->st_mode) ? 'w' : '-');
	ft_putchar((S_IXGRP & buffer->st_mode) ? 'x' : '-');
	last_bit(buffer);
}

char		*format_ls_str(int number, char *c)
{
	char	*ret;
	char	*ret_number;

	//dprintf(2, "NUMBERS:%d\n", number);
	ret_number = ft_itoa(number + 1);

	ret = ft_strjoin(" % ", ret_number);
	ret = ft_strjoin(ret, c);
	//"% 3d"
	//"% 5lld "
	//dprintf(2,"\t\t\tFORMAT:%s\n", ret);
	return (ret);
}

char		*format_name_str(int len, char *str)
{
	char *ret;

	ret = ft_strnjoin("%- ", ft_itoa(len), 2);
	ret = ft_strnjoin(ret, str, 1);
	return (ret);
}

void	print_number_of_links(int *format, struct stat *buffer)
{
	ft_printf(format_ls_str(format[0],"d "), buffer->st_nlink);
}

void	print_owner_name(int *format, struct stat *buffer)
{
	ft_printf(format_name_str(format[1], "s  "), getpwuid(buffer->st_uid)->pw_name);
}
	
void		print_owner_group(int *format, struct stat *buffer)
{
	ft_printf(format_name_str(format[2], "s"), getgrgid(buffer->st_gid)->gr_name);
}

void	print_file_size(int *format, struct stat *buffer)
{
		ft_printf(format_ls_str(format[3],"d "), buffer->st_size);
}

void	print_time_last_modified(struct stat *buffer)
{
	char	*time_str;

	time_str = &ctime(&buffer->st_mtimespec.tv_sec)[4];
	ft_bzero(&time_str[12], 1);
	ft_putstr(time_str);
}

void	print_symbolic_link(char *path)
{
	char	*sylink;
	int		count;

	count = 0;
	sylink = ft_strnew(1024);
	readlink(path, sylink, 1024);
	ft_printf(" -> %s", sylink);
}

char	*ft_strrnchr(char *str, char c, int count)
{
	while (count >= 0)
	{
		if (str[count] == c)
		{
			if (count == 0)
				return (str);
			else
				return (&str[count]);
		}
		count--;
	}
	return (NULL);
}

/*
 * this will handle all printing on the screen.
 *		given special flags, this will handle, long listing, reverse listing
 *		prints file name and if its a symbnolic link, prints the link info
*/

void	printer_scheme(char *str, struct stat *buffer, int flags, int *format)
{
	char	*file_name;
	
	file_name = ft_strrnchr(str, '/', ft_strlen(str)) + 1;
	if ((int)file_name == 1)
		file_name = str;
	// if (file_name == NULL || file_name[0] == '\0')
	// 	file_name = str;
	if ((flags & LONGFLG) == LONGFLG)
	{
		print_file_permissions(buffer);
		print_number_of_links(format, buffer);
		print_owner_name(format, buffer);
		print_owner_group(format, buffer);
		print_file_size(format, buffer);
		print_time_last_modified(buffer);
		ft_putchar(' ');
	}
	ft_printf("%s", file_name);
	if (S_ISLNK(buffer->st_mode))
		print_symbolic_link(file_name);
	ft_putchar('\n');
}

void	get_format_stats(t_list *head, int flags, int *format)
{
	struct stat buffer;
	int			temp;

	(void)flags;

	ft_bzero(format, sizeof(int) * 5);
	while (head != NULL)
	{
		if (lstat(head->content, &buffer) == 0)
		{
			format[4] += buffer.st_blocks;
			temp = ft_strlen(ft_itoa(buffer.st_nlink));
			if (format[0] < temp)
				format[0] = temp; 
			temp = ft_strlen(getpwuid(buffer.st_uid)->pw_name);
			if (format[1] < temp)
				format[1] = temp;
			temp  = ft_strlen(getgrgid(buffer.st_gid)->gr_name);
			if (format[2] < temp)
				format[2] = temp;
			temp = ft_strlen(ft_itoa(buffer.st_size));
			if (format[3] < temp)
				format[3] = temp;
			// temp = 
			// if (format[4] < temp)
			// 	temp = temp;
		}
		head = head->next;
	}
}


/*
 * Runs through lstat and sends files off to be printed.
 * 
*/

int	run_lstat(t_list *head, int flags)
{
	struct stat	buffer;
	t_list		*temp;
	int			format[5];
	int			ret;

	if (head == NULL)
		return (1);
	temp = head;
	ret = 0;
	get_format_stats(head, flags, format);
	if ((flags | LONGFLG) && !((flags & NODIR) == NODIR))
		ft_printf("total %d\n", format[4]);
	//file printing
	while(temp != NULL)
	{
		if (lstat(temp->content, &buffer) == 0)
		{
			// if (ft_strstr(temp->content, "/.") == NULL &&  ft_strstr(temp->content, "/..") == NULL)
			// {
				// if (ft_strrchr(temp->content, '/'))
				// 	printer_scheme(temp->content, &buffer, flags, format);
				// else
			if (S_ISDIR(buffer.st_mode) && ((flags & NODIR) == NODIR))
			{
				temp = temp->next;
				continue;
			}
					printer_scheme(temp->content, &buffer, flags, format);
			//}
		}
		else
		{
			ft_printf("ft_ls: %s: No such file or directory\n", temp->content);
			ret = 1;
		}
		temp = temp->next;
	}
	temp = head;
	while ((flags & RECFLG) == RECFLG && temp != NULL && (lstat(temp->content, &buffer) == 0))
	{
		if (S_ISDIR(buffer.st_mode))
		{
			if (ft_strstr(temp->content, "/.") == NULL &&  ft_strstr(temp->content, "/..") == NULL)
		 	{
			 	open_directory(temp->content, flags);
			}
		}
		temp = temp->next;
	}
	return (ret);
}

int		directory_file_read(t_list *head, int flags)
{
	struct stat buffer;
	t_list		*temp;
	t_list		*new;

	temp = head;
	while (temp != NULL && (lstat(temp->content, &buffer) == 0))
	{
		if (S_ISDIR(buffer.st_mode))
		{
			if (ft_strstr(temp->content, "/.") == NULL && ft_strstr(temp->content, "/..") == NULL)
		 	{
				new = open_directory(temp->content, flags);
				//if (not_solo_directory())
				//	ft_printf("%s:\n", directory);
				if ((flags & RECFLG) == RECFLG)
				{
					run_lstat(new, flags);
					directory_file_read(new, flags);
				}
			}
		}
		temp = temp->next;
	}
	return (0);
}

t_list		*open_directory(char *directory, int flags)
{
	DIR				*dir_fd;
	struct dirent	*buffer;
	t_list			*contents_list;
	char			*temp;

	contents_list = NULL;
	dir_fd = opendir(directory);
	if (dir_fd == NULL)
		return (NULL);	//find a way to handle this error
	while((buffer = readdir(dir_fd)) != NULL)
	{
		if ((flags & HIDFLG) != HIDFLG && buffer->d_name[0] == '.')
			continue;
		temp = ft_strjoin(directory, "/");
		temp = ft_strnjoin(temp, buffer->d_name, 1);
		contents_list = sorted_list(temp, flags, contents_list);
			free(temp);
	}
	closedir(dir_fd);
	return (contents_list);
}


int main(int argc, char **argv)
{
	int	flags;
	int	i;
	int ret;

	i = 0;
	ret = 0;
	flags = flag_checker(argv, &i);

//	t_list *head;
	if (flags < 0)
		return (1);
//	head = sorted_list(argc, argv, i, flags);
	if (i < argc)
	{
		ret = run_lstat(start_list(argc, argv, i, flags), flags + NODIR);
		//ret = directory_file_read(start_list(argc, argv, i, flags), flags);
		while (i < argc)
		{
			//if ((i + 1) == argc)
			//	flags += NODIR;
			ret = run_lstat(open_directory(argv[i], flags), flags + FIRSTDIR);
			i++;
			if (i != argc)
				ft_putchar('\n');

		}
	}
	else
	{
		ret = run_lstat(open_directory(".", flags), flags + FIRSTDIR);
		//ret = run_lstat(ft_lstnew(".", 2), flags);
		//ret = directory_file_read(ft_lstnew(".", 2), flags);
		//ret = run_lstat(start_list(argc, argv, i, flags), flags);
	}
	return (ret);
}

// while (head != NULL)
// 	{
// 		dprintf(2, "%s\n", head->content);
// 		head = head->next;
// 	}