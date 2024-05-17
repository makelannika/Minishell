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

	p = 0;
    if (!a[p])
        return (true);
    if (a[p] == '-')
        p++;
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
            {
                if(!(check_option(cmd[1])))
                    ft_printf(fd_out, "%s\n", cmd[i++]);
                else
                    ft_printf(fd_out, "%s", cmd[i++]);
            }
            else
                ft_printf(fd_out, "%s ", cmd[i++]);
        }
    }
}
