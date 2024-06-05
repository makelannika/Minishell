/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:33:21 by linhnguy          #+#    #+#             */
/*   Updated: 2024/06/05 17:35:12 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

_Bool	check_key_exist(char *env, char *cmd)
{
	if (ft_strncmp(env, cmd, ft_strlen(cmd)) == 0)
		return (1);
	else if (ft_strncmp(env, cmd, ft_strlen(cmd) - 1) == 0
		&& cmd[ft_strlen(cmd) - 1] == '=')
		return (0);
	return (0);
}

char	**putstr_in_array(t_pipex *data, char **env, char *str)
{
	char	**tmp;
	int		i;

	i = 0;
	tmp = env;
	env = malloc(sizeof(char *) *(array_len(env) + 2));
	if (!env)
		data->exitcode = -1;
	while (tmp[i])
	{
		env[i] = ft_strdup(tmp[i]);
		if (!env[i])
			data->exitcode = -1;
		free(tmp[i]);
		i++;
	}
	env[i++] = ft_strdup(str);
	env[i] = NULL;
	free(tmp);
	return (env);
}

_Bool	check_key(char *str)
{
	int	i;

	i = 1;
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalpha(str[i]) && str[i] != '_' && !ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	do_export(t_pipex *data, char **env, char **cmd, int fd_out)
{
	int	i;

	i = 0;
	if (!cmd[1])
		just_export_cmd(env, fd_out);
	else
	{
		i = 1;
		while (cmd[i])
		{
			if (!check_key(cmd[i]))
			{
				ft_printf(2,
					"export: `%s': not a valid identifier\n", cmd[i++]);
				continue ;
			}
			if (update_key(data, env, cmd[i]))
			{
				i++;
				continue ;
			}
			env = putstr_in_array(data, env, cmd[i++]);
		}
	}
	set_exitcode_and_env(data, env);
}
