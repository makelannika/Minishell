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

void	si_handler2(int signum)
{
	if (signum == SIGINT)
	{
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

static int	wait_children(int *pids, int count, int *exitcode)
{
	int					status;
	int					i;
	struct sigaction	si_data;

	i = 0;
	status = 0;
	si_data.sa_handler = SIG_IGN;
	si_data.sa_flags = 0;
	si_data.sa_mask = 0;
	sigaction(SIGQUIT, &si_data, NULL);
	sigaction(SIGINT, &si_data, NULL);
	while (i < count)
	{
		if (pids[i] == -1)
		{
			i++;
			continue ;
		}
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status) && i == count - 1)
			*exitcode = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*exitcode = WTERMSIG(status) + 128;
		if (*exitcode == 131)
			ft_putstr_fd("^\\Quit: 3\n", 2);
		else if (*exitcode == 130)
			ft_putstr_fd("^C\n", 2);
		i++;
	}
	si_data.sa_handler = si_handler2;
	sigaction(SIGINT, &si_data, NULL);
	return (*exitcode);
}

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

int	get_env(t_pipex *data)
{
	int			i;
	extern char	**environ;

	i = 0;
	while (environ[i])
		i++;
	data->env = ft_calloc(i + 1, sizeof(char *));
	if (!data->env)
		return (-1);
	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], "PWD=", 4) == 0)
			data->pwd = ft_strdup(environ[i]);
		else if (ft_strncmp(environ[i], "OLDPWD=", 7) == 0)
			data->oldpwd = ft_strdup(environ[i]);
		data->env[i] = ft_strdup(environ[i]);
		if (!data->env[i])
			return (-1);
		i++;
	}
	if (!data->pwd || !data->oldpwd)
		return (-1);
	return (0);
}

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
	data->execute = 0;
	return (0);
}

int	first_inits(t_pipex *data)
{
	if (!data->env)
	{
		if (get_env(data) == -1)
			return (free_first_inits(data));
	}
	if (!data->paths)
	{
		if (get_paths(data) == -1)
			return (free_first_inits(data));
	}
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
		if (data->execute)
		{
			if (do_process(data, processes) == -1
				|| (data->pids[data->count] == 0))
				return (close_and_free(data));
		}
		close(data->ends[0]);
		close(data->ends[1]);
		data->count++;
		processes = processes->next;
	}
	wait_children(data->pids, data->cmds, &data->exitcode);
	return (data->exitcode);
}
