/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:25:34 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/16 15:00:30 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

_Bool check_option(char *a)
{
	int p;

	p = 1;
    if (!a)
        return (true);
	while (a[p])
	{
		if (a[p] != 'n')
			return (false);
		p++;
	}
	return (true);
}
//FIXME: need to redo -n so that it prints all at once
void	do_echo(char **cmd, int fd_out)
{
	int		i;
	
	i = 1;
    while (cmd[i])
    {
        if (check_option(cmd[i]))
        {
            i++;
            continue ;
        }
        else
        {
            if (cmd[i + 1] == '\0')
                ft_printf(fd_out, "%s", cmd[i++]);
            else
                ft_printf(fd_out, "%s ", cmd[i++]);

        }
    }
	if(!(check_option(cmd[1])))
		ft_printf(fd_out, "\n");
}
