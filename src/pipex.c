/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 14:10:49 by linhnguy          #+#    #+#             */
/*   Updated: 2024/06/04 13:49:21 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	wait_children(t_pipex *data, int *pids, int count)
{
	int					status;
	int					i;

	i = 0;
	status = 0;
	ignore_signals(data);
	while (i < count)
	{
		if (pids[i] == -1)
		{
			i++;
			continue ;
		}
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			data->exitcode = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->exitcode = WTERMSIG(status) + 128;
		if (data->exitcode == 131)
			ft_putstr_fd("^\\Quit: 3\n", 2);
		else if (data->exitcode == 130)
			ft_putstr_fd("^C\n", 2);
		i++;
	}
	return (data->exitcode);
}

int	init_data(t_pipex *data, t_node *processes)
{
	if (get_paths(data) == -1)
		return (close_and_free(data));
	data->cmds = get_list_length(processes);
	data->count = 0;
	data->ends[0] = dup(STDIN_FILENO);
	data->ends[1] = dup(STDOUT_FILENO);
	data->read_end = -1;
	data->cmd_str = NULL;
	data->cmd = NULL;
	data->path = NULL;
	data->pids = ft_calloc(data->cmds, sizeof(int));
	if (!data->pids)
		return (close_and_free(data));
	data->pids[0] = -1;
	data->execute = 0;
	return (0);
}

int	pipex(t_node *processes, t_pipex *data)
{
	if (init_data(data, processes) == -1)
		return (set_exitcode(data, -1));
	while (data->count < data->cmds)
	{
		data->execute = check_cmd(processes->cmd);
		if (get_fds(data, processes) == -1)
			return (close_and_free(data));
		if (do_process(data, processes) == -1
			|| (data->pids[data->count] == 0))
			return (close_and_free(data));
		close(data->ends[0]);
		close(data->ends[1]);
		data->count++;
		processes = processes->next;
	}
	wait_children(data, data->pids, data->cmds);
	data->sa.sa_handler = si_handler;
	sigaction(SIGINT, &data->sa, NULL);
	return (data->exitcode);
}
