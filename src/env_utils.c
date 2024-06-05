/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:35:27 by amakela           #+#    #+#             */
/*   Updated: 2024/06/05 12:35:29 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
