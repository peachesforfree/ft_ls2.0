/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_func.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalcort <sbalcort@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/09 19:30:27 by sbalcort          #+#    #+#             */
/*   Updated: 2018/07/09 19:30:29 by sbalcort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_cont		*go_to_end(t_cont *temp)
{
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

int			multiple_dir(t_opndir *head)
{
	if (head->next != NULL)
		return (1);
	else if (head->last != NULL && head->last->path != NULL)
		return (1);
	return (0);
}

t_cont		*iterate(t_cont *current, int flags)
{
	if (flags & REVFLG)
		return (current->next);
	else
		return (current->last);
}

void		build_directory_chain(t_opndir *head, int flags)
{
	t_cont		*temp;
	t_opndir	*current;
	t_opndir	*dir_temp;

	current = head;
	if (current->dir_cont == NULL)
		return ;
	temp = head->dir_cont;
	if (!(flags & REVFLG))
		temp = go_to_end(current->dir_cont);
	while (temp != NULL)
	{
		if (temp->path != NULL)
		{
			lstat(temp->path, &temp->buffer);
			if (S_ISDIR(temp->buffer.st_mode) &&
			not_hidden_dir(head, temp, flags))
			{
				dir_temp = new_dir(temp->path);
				stack_opndir(current, dir_temp);
				populate_dir(dir_temp, flags);
			}
		}
		temp = iterate(temp, flags);
	}
}

void		run_stat_contents(t_cont *head)
{
	t_cont	*current;

	current = head;
	while (current)
	{
		lstat(current->path, &current->buffer);
		current = current->next;
	}
}
