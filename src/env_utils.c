/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:35:27 by amakela           #+#    #+#             */
/*   Updated: 2024/06/05 21:46:30 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	malloc_env(t_pipex *data)
{
	int			i;
	extern char	**environ;

	i = 0;
	while (environ[i])
		i++;
	data->env = ft_calloc(i + 1, sizeof(char *));
	if (!data->env)
		return (-1);
	return (0);
}

int	get_env(t_pipex *data)
{
	int			i;
	extern char	**environ;

	if (malloc_env(data) == -1)
		return (-1);
	i = 0;
	while (environ[i])
	{
		data->env[i] = ft_strdup(environ[i]);
		if (!data->env[i++])
		{
			free_str_array(data->env);
			return (-1);
		}
	}
	if (get_pwds(data) == -1)
		return (-1);
	return (0);
}

int	update_shlvl(char **env)
{
	int		shlvl;
	char	*new_shlvl;
	char	*tmp;

	shlvl = ft_atoi(*env + 6);
	shlvl++;
	new_shlvl = ft_itoa(shlvl);
	if (!new_shlvl)
		return (-1);
	tmp = ft_strjoin("SHLVL=", new_shlvl);
	if (!tmp)
	{
		free(new_shlvl);
		return (-1);
	}
	free(new_shlvl);
	free(*env);
	*env = tmp;
	return (0);
}

void	update_pwds(t_pipex *data, char **env, char *oldpwd)
{
	char	*newpwd;

	if (ft_strncmp(*env, "PWD=", 4) == 0
		|| (ft_strncmp(*env, "PWD", 3) == 0))
	{
		newpwd = getcwd(NULL, 0);
		if (!newpwd)
			return (set_error_and_print(data, -1, "2getcwd failed"));
		free(*env);
		*env = ft_strjoin("PWD=", newpwd);
		freeing(&newpwd, &data->pwd);
		data->pwd = ft_strdup(*env);
		if (!*env)
			return (set_error_and_print(data, -1, "strjoin failed"));
	}
	else if (ft_strncmp(*env, "OLDPWD=", 7) == 0
		|| (ft_strncmp(*env, "OLDPWD", 6) == 0))
	{
		free(*env);
		*env = ft_strjoin("OLDPWD=", oldpwd);
		freeing(&oldpwd, &data->oldpwd);
		data->oldpwd = ft_strdup(*env);
		if (!*env)
			return (set_error_and_print(data, -1, "strjoin failed"));
	}
}

_Bool	update_key(t_pipex *data, char **env, char *cmd)
{
	char	*equal;
	int		i;
	int		flag;

	i = 0;
	flag = 0;
	while (env[i])
	{
		if (check_key_exist(env[i], cmd))
			return (1);
		equal = ft_strchr(env[i], '=');
		if (ft_strncmp(env[i], cmd, equal - env[i] + 1) == 0
			|| (!equal && ft_strncmp(env[i], cmd, ft_strlen(env[i]))
				== 0 && cmd[ft_strlen(env[i])] == '='))
		{
			flag = 1;
			free(env[i]);
			env[i] = ft_strdup(cmd);
			if (!env[i])
				set_error_and_print(data, -1, "strdup failed in update_key");
		}
		i++;
	}
	return (flag);
}
