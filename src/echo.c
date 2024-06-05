/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:25:34 by linhnguy          #+#    #+#             */
/*   Updated: 2024/06/05 20:33:46 by linhnguy         ###   ########.fr       */
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

void	echo_empty_line(int fd_out, char *cmd, t_pipex *data)
{
	if (!cmd)
		ft_printf(fd_out, "\n");
	data->exitcode = 0;
}

char	*echo_one_string(char *option, char *cmd, int fd_out, t_pipex *data)
{
	char	*str;

	str = NULL;
	if (!(check_option(option)))
	{
		str = ft_strjoin(cmd, "\n");
		if (!str)
		{
			set_error_and_print(data, -1, "Malloc failed");
			return (NULL);
		}
		ft_putstr_fd(str, fd_out);
	}
	else
		ft_putstr_fd(cmd, fd_out);
	free (str);
	return (option);
}

char	*echo_multiple_strings(char *cmd, int fd_out, t_pipex *data)
{
	char	*str;

	str = ft_strjoin(cmd, " ");
	if (!str)
	{
		set_error_and_print(data, -1, "Malloc failed");
		return (NULL);
	}
	ft_putstr_fd(str, fd_out);
	free (str);
	return (cmd);
}

void	do_echo(char **cmd, int fd_out, t_pipex *data)
{
	int		i;

	i = 1;
	while (check_option(cmd[i]))
		i++;
	while (cmd[i])
	{
		if (cmd[i + 1] == '\0')
		{
			if (!(echo_one_string(cmd[1], cmd[i++], fd_out, data)))
				return ;
		}
		else
		{
			if (!(echo_multiple_strings(cmd[i++], fd_out, data)))
				return ;
		}
	}
	echo_empty_line(fd_out, cmd[1], data);
}
