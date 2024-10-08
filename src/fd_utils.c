/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:47:58 by amakela           #+#    #+#             */
/*   Updated: 2024/08/29 20:15:57 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	last_child(t_pipex *data, t_node *process)
{
	dup2(data->read_end, data->ends[0]);
	close(data->read_end);
	data->ends[1] = dup(STDOUT_FILENO);
	if (handle_heredocs(process, data) == -1)
		return (-1);
	return (handle_redirs(process, data));
}

static int	middle_child(t_pipex *data, t_node *process)
{
	int	tmp;

	if (pipe(data->ends) == -1)
	{
		ft_printf(2, "Minishell: error opening a pipe\n");
		return (set_exitcode(data, -1));
	}
	tmp = dup(data->read_end);
	dup2(data->ends[0], data->read_end);
	dup2(tmp, data->ends[0]);
	close(tmp);
	if (handle_heredocs(process, data) == -1)
		return (-1);
	return (handle_redirs(process, data));
}

static int	first_child(t_pipex *data, t_node *process)
{
	if (pipe(data->ends) == -1)
	{
		ft_printf(2, "Minishell: error opening a pipe\n");
		return (set_exitcode(data, -1));
	}
	data->read_end = dup(data->ends[0]);
	close(data->ends[0]);
	if (handle_heredocs(process, data) == -1)
		return (-1);
	return (handle_redirs(process, data));
}

int	get_fds(t_pipex *data, t_node *process)
{
	if (data->cmds == 1)
	{
		if (handle_heredocs(process, data) == -1)
			return (-1);
		return (handle_redirs(process, data));
	}
	if (data->count == 0)
		return (first_child(data, process));
	if (data->count == data->cmds - 1)
		return (last_child(data, process));
	else
		return (middle_child(data, process));
}
