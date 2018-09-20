#include "../includes/ft_ls.h"

t_cont		*iterate_t_cont(t_cont *temp, int flags)
{
	if (flags & REVFLG)
		return (temp->last);
	else
		return (temp->next);
}

void		link_name(t_cont *temp)
{
	char	*sylink;
	int		count;

	count = 0;
	sylink = ft_strnew(1024);
	readlink(temp->path, sylink, 1024);
	ft_printf(" -> %s", sylink);
}