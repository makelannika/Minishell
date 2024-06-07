/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:52:06 by amakela           #+#    #+#             */
/*   Updated: 2024/06/07 14:08:21 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	get_pwd(t_pipex *data, char	*env)
{
	data->pwd = ft_strdup(env);
	if (!data->pwd)
	{
		free_str_array((data->env));
		return (-1);
	}
	return (0);
}

static int	get_oldpwd(t_pipex *data, char **env)
{
	char	*tmp;

	tmp = *env;
	data->oldpwd = ft_strdup(*env);
	if (!data->oldpwd)
	{
		free(data->pwd);
		free_str_array(data->env);
		return (-1);
	}
	*env = ft_strdup("OLDPWD");
	free(tmp);
	if (!*env)
	{
		free(data->pwd);
		free(data->oldpwd);
		free_str_array(data->env);
		return (-1);
	}
	return (0);
}

int	get_pwds(t_pipex *data)
{
	int			i;

	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], "PWD=", 4) == 0)
		{
			if (get_pwd(data, data->env[i]) == -1)
				return (-1);
		}
		else if (ft_strncmp(data->env[i], "OLDPWD=", 7) == 0)
		{
			if (get_oldpwd(data, &data->env[i]) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}
