/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_item.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalcort <sbalcort@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/09 19:30:46 by sbalcort          #+#    #+#             */
/*   Updated: 2018/07/09 19:30:47 by sbalcort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

t_cont		*new_cont(char *path, t_cont *before, t_cont *after)
{
	t_cont	*temp;

	temp = (t_cont*)ft_memalloc(sizeof(t_cont));
	if (path)
		temp->path = ft_strdup(path);
	if (before != NULL)
	{
		temp->last = before;
		before->next = temp;
	}
	if (after != NULL)
	{
		temp->next = after;
		after->last = temp;
	}
	return (temp);
}

t_cont		*add_cont(char *path, t_cont *head, int flags)
{
	if (flags & TIMFLG)
		return (insert_time(path, head));
	else
		return (insert_alpha(path, head));
}

t_opndir	*new_dir(char *path)
{
	t_opndir	*temp;

	temp = ft_memalloc(sizeof(t_opndir));
	temp->path = ft_strdup(path);
	return (temp);
}

void remove_cont(t_opndir *head, char *path)
{
    t_cont *temp;

	temp = head->dir_cont;
	/*

this is where I last left off.
need to look through each element in the content of head.
will compare each content path with the path given.
when found, remove/free that one item and bridge the two before and after the removed element
keep into account if at end or the beggining of the end of the linked list

	*/

}

void		stack_opndir(t_opndir *current, t_opndir *new)
{
	t_opndir	*temp;

	temp = NULL;
	if (current->next != NULL)
		temp = current->next;
	current->next = new;
	new->last = current;
	if (temp != NULL)
	{
		temp->last = new;
		new->next = temp;
	}
	current->dir_cont = remove_cont(current->dir_cont, new->path);
}

void		enqueue_dir(t_opndir *head, t_opndir *new)
{
	t_opndir	*temp;

	temp = head;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	new->last = temp;
}
