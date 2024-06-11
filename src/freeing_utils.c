/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 17:52:16 by amakela           #+#    #+#             */
/*   Updated: 2024/06/07 14:07:00 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	free_env(t_pipex *data)
{
	if (data->env)
	{
		free_str_array(data->env);
		data->env = NULL;
	}
	if (data->pwd)
	{
		free(data->pwd);
		data->pwd = NULL;
	}
	if (data->oldpwd)
	{
		free(data->oldpwd);
		data->oldpwd = NULL;
	}
	return (-1);
}

void	free_str_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if (array[i])
			free(array[i++]);
	}
	free(array);
}

int	close_and_free(t_pipex *data)
{
	close(data->ends[0]);
	close(data->ends[1]);
	close(data->read_end);
	free_parent(data);
	free_env(data);
	return (-1);
}

void	free_parent(t_pipex *data)
{
	if (data->paths)
		free_str_array(data->paths);
	if (data->cmd_str)
		free(data->cmd_str);
	if (data->cmd)
		free_str_array(data->cmd);
	if (data->path)
		free(data->path);
	if (data->pids)
		free(data->pids);
	data->paths = NULL;
	data->cmd_str = NULL;
	data->cmd = NULL;
	data->path = NULL;
	data->pids = NULL;
}
