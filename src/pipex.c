/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 14:10:49 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/24 15:39:58 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	wait_children(int *pids, int count, int *exitcode)
{
	int	status;
	int	i;

	i = 0;
	status = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status) && i == count - 1)
			*exitcode = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*exitcode = WTERMSIG(status);
		i++;
	}
	return (*exitcode);
}

// adds a slash to the end of each path
static int	add_slash(t_pipex *data)
{
	int		i;
	char	*old;
	char	*new;

	i = 0;
	while (data->paths[i])
	{
		old = data->paths[i];
		new = ft_strjoin(old, "/");
		if (new == NULL)
			return (-1);
		free(old);
		data->paths[i] = new;
		i++;
	}
	return (0);
}

// saves an array of paths into pipex's struct
static int	get_paths(t_pipex *data)
{
	int			i;
	char		*envpaths;
	extern char	**environ;

	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], "PATH=", 4) == 0)
		{
			envpaths = ft_substr(environ[i], 5, ft_strlen(environ[i]) - 5);
			if (!envpaths)
				return (-1);
			data->paths = ft_split(envpaths, ':');
			free(envpaths);
			if (!data->paths)
				return (-1);
			if (add_slash(data) == -1)
				return (-1);
			return (0);
		}
		i++;
	}
	data->paths = NULL;
	return (0);
}

// saves a copy of env into pipex's struct
int	get_env(t_pipex *data)
{
	int			i;
	extern char	**environ;

	i = 0;
	while (environ[i])
		i++;
	data->env = ft_calloc(i + 1, sizeof(char *));
	if (!data->env)
	{
		data->exitcode = -1;
		return (-1);
	}
	i = 0;
	while (environ[i])
	{
		data->env[i] = ft_strdup(environ[i]);
		i++;
	}
	return (0);
}

// initializes pipex's struct
int	init_data(t_pipex *data, t_node *processes)
{
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
	data->exitcode = 0;
	return (0);
}

// initializes env and path in the main
int	first_inits(t_pipex *data)
{
	if (!data->env)
	{
		if (get_env(data) == -1)
		return (-1);
	}
	if (!data->paths)
	{
		if (get_paths(data) == -1)
		{
			free_str_array(data->env);
			return (-1);
		}
	}
	return (0);
}

int	pipex(t_node *processes, t_pipex *data)
{	
	if (init_data(data, processes) == -1)
		return (set_exitcode(data, -1));
	while (data->count < data->cmds)
	{
		data->exitcode = 0;
		if (get_fds(data, processes) == -1)
			return (close_and_free(data));
		if (data->exitcode == 0)
		{
			if (forking(data, processes) == -1
				|| (data->pids[data->count] == 0))
					return (close_and_free(data));
		}
		close(data->ends[0]);
		close(data->ends[1]);
		data->count++;
		processes = processes->next;
	}
	wait_children(data->pids, data->cmds, &data->exitcode);
	return (0);
}
