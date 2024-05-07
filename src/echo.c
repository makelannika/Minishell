/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:25:34 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/07 13:57:05 by linhnguy         ###   ########.fr       */
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

void	do_echo(char **cmd)
{
	int		i;
	
	i = 1;
    if (cmd[i] && cmd[i][0] != '-')
    {
        ft_printf(1, "%s", cmd[1]);
        ft_printf(1, "\n");
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
                    ft_printf(1, "%s", cmd[i++]);
                return ;
            }
        }
    }
	if(!(check_option(cmd[1])))
		ft_printf(1, "\n");
}
