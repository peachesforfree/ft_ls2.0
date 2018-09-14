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

#include "../includes/ft_ls.h"

t_cont		*go_to_end(t_cont *temp)
{
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

int			multiple_dir(t_opndir *head)
{
	
	if (head->next != NULL)
	{
		//dprintf(2, "MULTDIR:head->path:%s\thead->next:%p\thead->last:%p\t RET;1\n", head->path, head->next, head->last);
		return (1);
	}
	if (head->last != NULL) //&& head->last->path != NULL)
	{
		//dprintf(2, "MULTDIR:head->path:%s\thead->next:%p\thead->last:%p\t ret:1\n", head->path, head->next, head->last);
			return (1);
	}
	//dprintf(2, "MULTDIR:head->path:%s\thead->next:%p\thead->last:%p\tret:0\n", head->path, head->next, head->last);
	return (0);
}

t_cont		*iterate(t_cont *current, int flags)
{
	if (flags & REVFLG)
		return (current->next);
	else
		return (current->last);
}


void remove_cont(t_opndir *head, char *path)
{
    t_cont	*temp;
	t_cont	*current;
	
	if (head->path != NULL || !head || !path)
		return;
	current = head->dir_cont;
	while (current)
	{
		if (!ft_strcmp(current->path, path))
			break;
			//dprintf(2,"PATH GIVEN:%s\tNMEED:%s\n", temp->path, path);
		current = current->next;
	}
	if (current == NULL)
		return;
	temp = head->dir_cont;
	while (temp != NULL)
	{
		//if selected is in front
		if (temp == current && temp->last == NULL)
		{
			//dprintf(2,"FRONT\tTEMP:%p\tCURRENT:%p\n", temp, current);
			head->dir_cont = current->next;
			//free(current);
			if (current->next != NULL)
				current->next->last = NULL;
			return;
		}
		//if selected is in middle
		if (temp == current && temp->next != NULL)
		{
			//dprintf(2,"MIDDLE\tTEMP:%p\tCURRENT:%p\n", temp, current);
			if (current->last != NULL)
				current->last->next = current->next;
			if (current->next != NULL)
				current->next->last = current->last;
			//free(current);
			return;
		}
		//if selected i at end
		if (temp == current && temp->next == NULL)
		{
			//dprintf(2,"END\tTEMP:%p\tCURRENT:%p\n", temp, current);
			if (current->last != NULL)
				current->last->next = NULL;
			//free(current);
			return ;
		}		
		temp = temp->next;
	}
}

void		rm_cont(t_cont *current)
{
	if (current->last != NULL)
		current->last->next = current->next;
	if (current->next != NULL)
		current->next->last = current->last;
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
	if (!(flags & REVFLG) && temp != NULL)
	{
		temp = go_to_end(current->dir_cont);
	}
	//print_full_chain(head);
	while (temp != NULL)
	{
			lstat(temp->path, &temp->buffer);
			if (S_ISDIR(temp->buffer.st_mode))// && 
			//not_hidden_dir(head, temp, flags))
			{
				dir_temp = new_dir(temp->path);
				stack_opndir(current, dir_temp);
				populate_dir(dir_temp, flags);
				rm_cont(temp);
				//remove_cont(head, temp->path);
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
