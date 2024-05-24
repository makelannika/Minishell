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
static int	last_child(t_pipex *data, t_node *processes)
{
	dup2(data->read_end, data->ends[0]);
	close(data->read_end);
	// dup2(STDOUT_FILENO, data->ends[1]);
	data->ends[1] = dup(STDOUT_FILENO);
	handle_redirs(processes, data);
	return (0);
}

// opens and closes correct fds for middle child processes
static int	middle_child(t_pipex *data, t_node *processess)
{
	int	tmp;

	if (pipe(data->ends) == -1)
	{
		ft_printf(2, "Error opening a pipe\n");
		data->exitcode = -1;
		return (close_and_free(data));
	}
	tmp = dup(data->read_end);
	dup2(data->ends[0], data->read_end);
	dup2(tmp, data->ends[0]);
	close(tmp);
	handle_redirs(processess, data);
	return (0);
}

// opens and closes correct fds for first child process
static int	first_child(t_pipex *data, t_node *processes)
{
	if (pipe(data->ends) == -1)
	{
		ft_printf(2, "Error opening a pipe\n");
		return (set_exitcode(data, -1));
	}
	data->read_end = dup(data->ends[0]);
	close(data->ends[0]);
	handle_redirs(processes, data);
	return (0);
}

// opens and closes correct fds based on the process
int	get_fds(t_pipex *data, t_node *processes)
{
	if (data->cmds == 1)
	{
		handle_redirs(processes, data);
		return (data->exitcode);
	}
	if (data->count == 0)
		return (first_child(data, processes));
	if (data->count == data->cmds - 1)
		return (last_child(data, processes));
	else
		return (middle_child(data, processes));
}
