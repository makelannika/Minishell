/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 16:26:09 by amakela           #+#    #+#             */
/*   Updated: 2024/06/07 14:06:28 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	set_exitcode(t_pipex *data, int exitcode)
{
	data->exitcode = exitcode;
	return (-1);
}

void	do_dups(t_pipex *data)
{
	close(data->read_end);
	dup2(data->ends[0], STDIN_FILENO);
	dup2(data->ends[1], STDOUT_FILENO);
	close(data->ends[0]);
	close(data->ends[1]);
}

static int	do_cmd(t_pipex *data, t_node *process)
{
	if (!process->builtin)
		do_dups(data);
	if (parse_cmd(data, &process->cmd) == -1)
		return (-1);
	if (is_builtin(data->cmd[0]))
		return (call_builtin(data, data->cmd[0]));
	if (get_path(data) == -1)
		return (-1);
	execve(data->path, data->cmd, data->env);
	if (access(data->cmd[0], F_OK) == 0)
	{
		ft_printf(2, "MOOshell: %s: is a directory\n", data->cmd[0]);
		return (set_exitcode(data, 126));
	}
	ft_printf(2, "MOOshell: %s: Permission denied\n", data->cmd[0]);
	return (set_exitcode(data, 1));
}

int	do_process(t_pipex *data, t_node *process)
{
	if (data->cmds == 1 && process->builtin)
	{
		if (!data->execute)
			return (data->exitcode);
		if (do_cmd(data, process) == -1)
			return (-1);
	}
	else
	{
		data->pids[data->count] = fork();
		if (data->pids[data->count] < 0)
		{
			ft_printf(2, "MOOshell: error: fork failed\n");
			return (set_exitcode(data, -1));
		}
		if (data->pids[data->count] == 0)
		{
			data->sa.sa_handler = SIG_DFL;
			sigaction(SIGQUIT, &data->sa, NULL);
			if (!data->execute || do_cmd(data, process) == -1)
				return (-1);
		}
	}
	return (0);
}
