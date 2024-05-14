/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:25:34 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/14 15:12:16 by amakela          ###   ########.fr       */
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
    if (cmd[i] && cmd[i][0] != '-')
    {
        ft_printf(fd_out, "%s\n", cmd[1]);
        return ;
    }
    else
    {
        while (cmd[i])
        {
            if (check_option(cmd[i]))
                i++;
            else
            {
                while (cmd[i])
                    ft_printf(fd_out, "%s", cmd[i++]);
                return ;
            }
        }
    }
	if(!(check_option(cmd[1])))
		ft_printf(fd_out, "\n");
}
