/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 21:46:58 by amakela           #+#    #+#             */
/*   Updated: 2024/06/04 21:47:00 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static _Bool	check_case(char *cmd, char *builtin)
{
	int		i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] != builtin[i] && cmd[i] + 32 != builtin[i])
			return (0);
		i++;
	}
	if (cmd[i] != builtin[i])
		return (0);
	return (1);
}

int	call_builtin(t_pipex *data, char *cmd)
{
	if (ft_strncmp(cmd, "cd", 3) == 0)
		do_cd(data, data->cmd, data->env);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		do_export(data, data->env, data->cmd, data->ends[1]);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		do_unset(data->env, data->cmd, data);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		do_exit(data->cmd, data);
	else if (cmd[0] == 'p' || cmd[0] == 'P')
	{
		if (check_case(cmd, "pwd"))
			put_pwd(data, data->ends[1]);
	}
	else if (cmd[0] == 'e' || cmd[0] == 'E')
	{
		if (check_case(cmd, "env"))
			put_env(data->env, data->ends[1], data);
		else if (check_case(cmd, "echo"))
			do_echo(data->cmd, data->ends[1], data);
	}
	return (data->exitcode);
}

_Bool	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "cd\0", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "export\0", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset\0", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit\0", 5) == 0)
		return (1);
	if (cmd[0] == 'p' || cmd[0] == 'P')
		return (check_case(cmd, "pwd\0"));
	if (cmd[0] == 'e' || cmd[0] == 'E')
	{
		if (check_case(cmd, "env\0")
			|| check_case(cmd, "echo\0"))
			return (1);
	}
	return (0);
}
