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

int		error_no_option(char c)
{
	ft_printf("ft_ls: %c: Does not exist\n", c);
	return (-1);
}

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

int			flag_checker(char **argv, int *i)
{
	int		result;
	int		x;

	result = 0;
	while (argv[++*i] != NULL)
	{
		x = 0;
		if (argv[*i][0] == '-')
		{
			x++;
			while (argv[*i][x] != '\0' && ft_strchr(FLAGCHAR, argv[*i][x]))
			{
				result |= bit_stuff(argv[*i][x]);
				x++;
			}
			if (argv[*i][x] != '\0' && (argv[*i][0] == '-') &&
			(ft_strchr(FLAGCHAR, argv[*i][x]) == NULL))
				return(error_no_option(argv[*i][x]));
		}
		else
			return (result);
	}
	return (result);
}
