/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 21:43:23 by amakela           #+#    #+#             */
/*   Updated: 2024/06/04 21:43:25 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	ft_printf(2, "MOOshell: %s: command not found\n", data->cmd[0]);
	return (set_exitcode(data, 127));
}

static int	path_check(t_pipex *data)
{
	int	start;

	start = ft_strlen(data->cmd[0]);
	while (data->cmd[0][start] != '/')
		start--;
	if (is_builtin(&data->cmd[0][start + 1]))
	{
		ft_printf(2, "MOOshell: %s: No such file or directory\n", data->cmd[0]);
		return (set_exitcode(data, 127));
	}
	if (access(data->cmd[0], F_OK) == 0)
	{
		data->path = ft_substr(data->cmd[0], 0, ft_strlen(data->cmd[0]));
		if (!data->path)
			return (set_exitcode(data, -1));
		return (0);
	}
	ft_printf(2, "MOOshell: %s: No such file or directory\n", data->cmd[0]);
	return (set_exitcode(data, 127));
}

int	get_path(t_pipex *data)
{
	if (ft_strchr(data->cmd[0], '/'))
	{
		if (path_check(data) == -1)
			return (-1);
	}
	else
	{
		if (find_path(data) == -1)
			return (-1);
	}
	return (0);
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

int	get_paths(t_pipex *data)
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
