/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbalcort <sbalcort@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/09 19:30:16 by sbalcort          #+#    #+#             */
/*   Updated: 2018/07/09 19:30:17 by sbalcort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int			bit_stuff(char c)
{
	int		result;

	result = 0;
	result |= (c == 'l') ? LONGFLG : 0;
	result |= (c == 'R') ? RECFLG : 0;
	result |= (c == 'a') ? HIDFLG : 0;
	result |= (c == 'r') ? REVFLG : 0;
	result |= (c == 't') ? TIMFLG : 0;
	return (result);
}

int			flag_checker(char **argv)
{
	int		result;
	int		y;
	int		x;

	result = 0;
	y = 0;
	while (argv[++y] != NULL)
	{
		x = 0;
		if (argv[y][0] == '-')
		{
			x++;
			while (argv[y][x] != '\0' && ft_strchr(FLAGCHAR, argv[y][x]))
			{
				result |= bit_stuff(argv[y][x]);
				x++;
			}
			if (argv[y][x] != '\0' && (argv[y][0] == '-') &&
			!ft_strchr(FLAGCHAR, argv[y][x]))
				error_no_option(argv[y][x]);
		}
		else
			return (result);
	}
	return (result);
}
