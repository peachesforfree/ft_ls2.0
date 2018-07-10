/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_algorithms.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalcort <sbalcort@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/09 19:31:24 by sbalcort          #+#    #+#             */
/*   Updated: 2018/07/09 19:31:25 by sbalcort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_cont		*insert_time(char *path, t_cont *head)
{
	t_cont	*temp;
	t_cont	*current;

	current = head;
	temp = new_cont(path, NULL, NULL);
	lstat(temp->path, &temp->buffer);
	if (current == NULL)
		return (temp);
	while (current != NULL)
	{
		if (current->buffer.st_mtimespec.tv_sec < temp->buffer.st_mtimespec.tv_sec
		|| ((current->buffer.st_mtimespec.tv_sec == temp->buffer.st_mtimespec.tv_sec)
		&& (current->buffer.st_mtimespec.tv_nsec < temp->buffer.st_mtimespec.tv_nsec)))
		{
			if (current->last == NULL)
			{
				temp->next = current;
				current->last = temp;
				return (temp);
			}
			else if (current->last != NULL)
			{
				temp->last = current->last;
				temp->next = current;
				temp->last->next = temp;
				temp->next->last = temp;
				return (head);
			}
		}
		if (current->next == NULL)
		{
			temp->last = current;
			current->next = temp;
			return (head);
		}
		current = current->next;
	}
	return (head);
}


t_cont		*insert_alpha(char *path, t_cont *head)
{
	t_cont	*current;

	if (head == NULL)
		return (new_cont(path, NULL, NULL));
	current = head;
	while (current)
	{
		if (ft_strcmp(current->path, path) >= 0)
		{
			if (current->last == NULL)
			{
				head = new_cont(path, NULL, current);
				break ;
			}
			else
			{
				new_cont(path, current->last, current);
				break ;
			}
		}
		else if (ft_strcmp(current->path, path) < 0)
		{
			if (current->next == NULL)
			{
				new_cont(path, current, NULL);
				break ;
			}
		}
		current = current->next;
	}
	return (head);
}
