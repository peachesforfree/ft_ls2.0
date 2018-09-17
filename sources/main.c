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

void	open_directory(char *directory, int flags);

/*
	problem with printing results

	need to look into the stat buffer in void		get_format_stats(t_opndir *master) in print_files.c
	wrong value from struct is being used and need to find the right one.
	its screwing up values in the moulitest

*/

#include "../includes/ft_ls.h"


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
	//else
	//	head = append(head, str, ft_strlen(str), &insert_time);			//insert time
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



int	run_lstat(t_list *head, int flags)
{
	struct stat	buffer;
	t_list		*temp;
	(void)flags;

	temp = head;
	//file printing
	while(temp != NULL)
	{
		if (lstat(temp->content, &buffer) == 0)
		{
			if (ft_strstr(temp->content, "/.") == NULL &&  ft_strstr(temp->content, "/..") == NULL)
			{
				if (ft_strrchr(temp->content, '/'))
					ft_printf("%s\n", ft_strrchr(temp->content, '/') + 1);
				else
					ft_printf("%s\n", temp->content);
			}
		}
		else
				ft_printf("ft_ls: %s: No such file or directory\n", temp->content);
		temp = temp->next;
	}
	temp = head;
	while (temp != NULL && (lstat(temp->content, &buffer) == 0))
	{
		if (S_ISDIR(buffer.st_mode))
		{
			if (ft_strstr(temp->content, "/.") == NULL &&  ft_strstr(temp->content, "/..") == NULL)
		 		open_directory(temp->content, flags);	//somewhere in here we use recursive flag
		}
		temp = temp->next;
	}
	return (0);
}

void	open_directory(char *directory, int flags)
{
	DIR				*dir_fd;
	struct dirent	*buffer;
	t_list			*contents_list;
	char			*temp;

	contents_list = NULL;
	dir_fd = opendir(directory);
	if (dir_fd == NULL)
		return ;	//find a way to handle this error
	ft_printf("\n%s:\n", directory);
	while((buffer = readdir(dir_fd)) != NULL)
	{
		temp = ft_strjoin(directory, "/");
		temp = ft_strnjoin(temp, buffer->d_name, 1);
		contents_list = sorted_list(temp, flags, contents_list);
			free(temp);
	}
	closedir(dir_fd);
	run_lstat(contents_list, flags);
}


int main(int argc, char **argv)
{
	int	flags;
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	flags = flag_checker(argv, &i);

//	t_list *head;
	if (flags < 0)
		return (1);
//	head = sorted_list(argc, argv, i, flags);
	if (i < argc)
		ret = run_lstat(start_list(argc, argv, i, flags), flags);
	else
		ret = run_lstat(ft_lstnew(".", 2), flags);
	return (ret);
}

// while (head != NULL)
// 	{
// 		dprintf(2, "%s\n", head->content);
// 		head = head->next;
// 	}