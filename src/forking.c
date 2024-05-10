/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 16:26:09 by amakela           #+#    #+#             */
/*   Updated: 2024/05/10 21:10:10 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// checks all case combinations for builtin commands
// is called only for pwd, env & echo
_Bool	check_case(char *cmd, char *builtin)
{
	int		i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] != builtin[i] && cmd[i] + 32 != builtin[i])
			return (false);
		i++;
	}
	if (cmd[i] != builtin[i])
		return (false);
	return (true);
}

// checks if command is a builtin
_Bool	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "cd\0", 3) == 0)
		return (true);
	if (ft_strncmp(cmd, "export\0", 7) == 0)
		return (true);
	if (ft_strncmp(cmd, "unset\0", 6) == 0)
		return (true);
	if (ft_strncmp(cmd, "exit\0", 5) == 0)
		return (true);
	if (cmd[0] == 'p' || cmd[0] == 'P')
		return (check_case(cmd, "pwd\0"));
	if (cmd[0] == 'e' || cmd[0] == 'E')
	{
		if (check_case(cmd, "env\0")
			|| check_case(cmd, "echo\0"))
			return (true);
	}
	return (false);
}

// finds correct path for cmd / checks if command exists
static int	find_path(t_pipex *data)
{
	int		i;
	char	*temp;

	i = 0;
	if (data->paths)
	{
		while (data->paths[i])
		{
			temp = ft_strjoin(data->paths[i], data->cmd[0]);
			if (!temp)
				return (-1);
			if (access(temp, F_OK) == 0)
			{
				data->path = temp;
				return (0);
			}
			free(temp);
			i++;
		}
	}
	ft_printf(2, "command not found: %s\n", data->cmd[0]);
	close_and_free(data);
	exit(127);
}

// checks path/cmd when relative/absolute path is given
// gives an error if cmd is a builtin
static int	path_check(t_pipex *data)
{
	int	start;
	
	start = ft_strlen(data->cmd[0]);
	while (data->cmd[0][start] != '/')
		start--;
	if (is_builtin(&data->cmd[0][start + 1]))
	{
		ft_printf(2, "no such file or directory: %s\n", data->cmd[0]);
		return (-1);
	}
	if (access(data->cmd[0], F_OK) == 0)
	{
		data->path = ft_substr(data->cmd[0], 0, ft_strlen(data->cmd[0]));
		return (0);
	}
	ft_printf(2, "no such file or directory: %s\n", data->cmd[0]);
	return (-1);
}

// gets path for cmd if needed & calls builtin functions
static int	get_path(t_pipex *data)
{
	if (ft_strchr(data->cmd[0], '/'))
	{
		if (path_check(data) == -1)
			return (-1);
	}
	else if (is_builtin(data->cmd[0]))
	{
		// call specific builtin, args: t_data *pipex
		return (-1);
	}
	else
	{
		if (find_path(data) == -1)
			return (-1);
	}
	return (0);
}

// gets cmd array
static int	get_cmd(char *cmd, t_pipex *data)
{
	data->cmd = ft_split(cmd, 7);
	if (!data->cmd)
	{
		ft_printf(2, "Error\nSplit failed when getting a command\n");
		return (-1);
	}
	if (data->cmd[0] == '\0')
	{
		ft_printf(2, "command not found: %s\n", cmd);
		close_and_free(data);
		exit(127);
	}
	if (get_path(data) == -1)
		return (-1);
	return (0);
}

// execve here
static int	do_cmd(t_pipex *data, t_node *processes)
{
	extern char	**environ;

	if (data->read_end != -1)
		close(data->read_end);
	dup2(data->ends[0], STDIN_FILENO);
	dup2(data->ends[1], STDOUT_FILENO);
	if (data->ends[0] != -1)
		close(data->ends[0]);
	if (data->ends[1] != -1)
		close(data->ends[1]);
	parse_cmd(data, processes->cmd);
	if (!data->cmd_str)
		return (-1);
	if (get_cmd(data->cmd_str, data) == -1)
		return (-1);
	if (!data->path)
	{
		ft_printf(2, "command not found: %s\n", data->cmd[0]);
		return (-1);
	}
	execve(data->path, data->cmd, environ);
	ft_printf(2, "permission denied: %s\n", data->cmd[0]);
	return (-1);
}

// returns a string containing only the cmd from the cmd string
// needed to check if the cmd is a builtin and we don't want to fork
char	*trim_cmd(char *cmd_str, char **trimmed)
{
	int		i;
	int		len;
	int		start;

	i = 0;
	len = 0;
	start = 0;
	while (cmd_str[i] && cmd_str[i] == ' ')
		i++;
	start = i;
	while (cmd_str[i] && cmd_str[i] != ' ')
	{
		i++;
		len++;
	}
	*trimmed = ft_substr(cmd_str, start, len);
	if (!trimmed)
		return (NULL);
	return (*trimmed);
}

// forks, unless there's only one cmd and it is a builtin
int	forking(t_pipex *data, t_node *processes)
{
	char	*trimmed;

	trimmed = NULL;
	if (trim_cmd(processes->cmd, &trimmed) == NULL)
		return (-1);
	if (data->cmds == 1 && is_builtin(trimmed))
	{
		free(trimmed);
		if (do_cmd(data, processes) == -1)
			return (0);
	}
	else
	{
		free(trimmed);
		data->pids[data->count] = fork();
		if (data->pids[data->count] < 0)
		{
			ft_printf(2, "Error\nFork failed\n");
			return (-1);
		}
		if (data->pids[data->count] == 0)
		{
			if (do_cmd(data, processes) == -1)
				return (-1);
		}
	}
	return (0);
}