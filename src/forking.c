/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 16:26:09 by amakela           #+#    #+#             */
/*   Updated: 2024/05/24 15:22:07 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	set_exitcode(t_pipex *data, int exitcode)
{
	data->exitcode = exitcode;
	return (-1);
}

// checks all case combinations for builtin commands
// is called only for pwd, env & echo
_Bool	check_case(char *cmd, char *builtin)
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

// calls the correct builtin function
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

// checks if command is a builtin
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
				return (set_exitcode(data, -1));
			if (access(temp, F_OK) == 0)
			{
				data->path = temp;
				return (0);
			}
			free(temp);
			i++;
		}
	}
	ft_printf(2, "MOOshell: command not found: %s\n", data->cmd[0]);
	close_and_free(data);
	return (set_exitcode(data, 127));
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
		ft_printf(2, "MOOshell: no such file or directory: %s\n", data->cmd[0]);
		return (set_exitcode(data, 127));
	}
	if (access(data->cmd[0], F_OK) == 0)
	{
		data->path = ft_substr(data->cmd[0], 0, ft_strlen(data->cmd[0]));
		return (0);
	}
	ft_printf(2, "MOOshell: no such file or directory: %s\n", data->cmd[0]);
	return (set_exitcode(data, 127));
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
		call_builtin(data, data->cmd[0]);
		return (data->exitcode);
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
		ft_printf(2, "MOOshell: error: split failed\n");
		return (set_exitcode(data, -1));
	}
	if (data->cmd[0] == '\0')
	{
		ft_printf(2, "MOOshell: command not found: %s\n", cmd);
		close_and_free(data);
		return (set_exitcode(data, 127));
	}
	if (get_path(data) == -1)
		return (-1);
	return (0);
}

// execve here
static int	do_cmd(t_pipex *data, t_node *process)
{
	if (!process->builtin)
	{
		close(data->read_end);
		dup2(data->ends[0], STDIN_FILENO);
		dup2(data->ends[1], STDOUT_FILENO);
		close(data->ends[0]);
		close(data->ends[1]);
	}
	if (parse_cmd(data, &process->cmd) == -1)
		return (-1);
	if (process->builtin)
		return (get_cmd(data->cmd_str, data));
	if (get_cmd(data->cmd_str, data) == -1)
		return (-1);
	if (!data->path)
	{
		ft_printf(2, "MOOshell: command not found: %s\n", data->cmd[0]);
		return (set_exitcode(data, 127));
	}
	execve(data->path, data->cmd, data->env);
	ft_printf(2, "MOOshell: permission denied: %s\n", data->cmd[0]);
	return (set_exitcode(data, 1));
}

// returns a string containing only the cmd from the cmd string
// needed to check if the cmd is a builtin and we don't want to fork
char	*trim_cmd(char *cmd_str)
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
	return (ft_substr(cmd_str, start, len));
}

int	forking(t_pipex *data, t_node *process)
{
	if (data->cmds == 1 && process->builtin)
	{
		// ft_printf(2, "not forking\n");
		if (do_cmd(data, process) == -1)
			return (-1);
	}
	else
	{
		// ft_printf(2, "forking\n");
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
			if (do_cmd(data, process) == -1)
				return (-1);
		}
	}
	return (0);
}
