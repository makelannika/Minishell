/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:47:58 by amakela           #+#    #+#             */
/*   Updated: 2024/05/24 15:52:08 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// opens and closes correct fds for last child process
static int	last_child(t_pipex *data, t_node *process)
{
	dup2(data->read_end, data->ends[0]);
	close(data->read_end);
	data->ends[1] = dup(STDOUT_FILENO);
	if (handle_heredocs(process, data) == -1)
		return (-1);
	return (handle_redirs(process, data));
}

// opens and closes correct fds for middle child processes
static int	middle_child(t_pipex *data, t_node *process)
{
	int	tmp;

	if (pipe(data->ends) == -1)
	{
		ft_printf(2, "MOOshell: error opening a pipe\n");
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

// opens and closes correct fds for first child process
static int	first_child(t_pipex *data, t_node *process)
{
	if (pipe(data->ends) == -1)
	{
		ft_printf(2, "MOOshell: error opening a pipe\n");
		return (set_exitcode(data, -1));
	}
	data->read_end = dup(data->ends[0]);
	close(data->ends[0]);
	if (handle_heredocs(process, data) == -1)
		return (-1);
	return (handle_redirs(process, data));
}

// opens and closes correct fds based on the process
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
