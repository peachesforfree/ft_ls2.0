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

int			derps(char *path, t_cont *current)
{
	if (current->next == NULL)
	{
		new_cont(path, current, NULL);
		return (1);
	}
	return (0);
}

int			ferps(t_cont *current, char *path, t_cont *head)
{
	if (current->last == NULL)
	{
		head = new_cont(path, NULL, current);
		return (1);
	}
	return (0);
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
			if (ferps(current, path, head))
				break ;
			else
			{
				new_cont(path, current->last, current);
				break ;
			}
		}
		else if (ft_strcmp(current->path, path) < 0)
			if (derps(path, current))
				break ;
		current = current->next;
	}
	return (head);
}
