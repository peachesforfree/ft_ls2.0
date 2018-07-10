/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_sort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalcort <sbalcort@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/09 23:58:38 by sbalcort          #+#    #+#             */
/*   Updated: 2018/07/09 23:58:40 by sbalcort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_cont		*secondary(t_cont *temp, t_cont *current, t_cont *head)
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
	return (NULL);
}

int			time_check(t_cont *current, t_cont *temp)
{
	if (current->buffer.st_mtimespec.tv_sec < temp->buffer.st_mtimespec.tv_sec
	|| ((current->buffer.st_mtimespec.tv_sec ==
	temp->buffer.st_mtimespec.tv_sec) &&
	(current->buffer.st_mtimespec.tv_nsec < temp->buffer.st_mtimespec.tv_nsec)))
		return (1);
	return (0);
}

void		bridge(t_cont *temp, t_cont *current)
{
	temp->last = current;
	current->next = temp;
}

t_cont		*insert_time(char *path, t_cont *head)
{
	t_cont	*temp;
	t_cont	*current;
	t_cont	*holder;

	current = head;
	temp = new_cont(path, NULL, NULL);
	lstat(temp->path, &temp->buffer);
	if (current == NULL)
		return (temp);
	while (current != NULL)
	{
		if (time_check(current, temp))
		{
			holder = secondary(temp, current, head);
			if (holder != NULL)
				return (holder);
		}
		if (current->next == NULL)
		{
			bridge(temp, current);
			return (head);
		}
		current = current->next;
	}
	return (head);
}
