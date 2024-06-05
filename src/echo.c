/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:25:34 by linhnguy          #+#    #+#             */
/*   Updated: 2024/06/05 16:20:17 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

_Bool	check_option(char *a)
{
	int	i;

	i = 0;
	if (!a[i])
		return (1);
	if (a[i] != '-')
		return (0);
	if (a[i] == '-')
		i++;
	while (a[i])
	{
		if (a[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	echo_empty_line(int fd_out, char *cmd)
{
	if (!cmd)
		ft_printf(fd_out, "\n");
}

void	do_echo(char **cmd, int fd_out, t_pipex *data)
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
				if (!(check_option(cmd[1])))
					ft_printf(fd_out, "%s\n", cmd[i++]);
				else
					ft_printf(fd_out, "%s", cmd[i++]);
			}
			else
				ft_printf(fd_out, "%s ", cmd[i++]);
		}
	}
	echo_empty_line(fd_out, cmd[1]);
	data->exitcode = 0;
}
