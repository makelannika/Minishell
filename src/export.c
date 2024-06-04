/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:33:21 by linhnguy          #+#    #+#             */
/*   Updated: 2024/06/04 14:01:56 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void static	swap_strings(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	sort_strings(char **arr)
{
	int	swapped;
	int	i;
	int	size;

	size = array_len(arr);
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (i < size - 1)
		{
			if (ft_strncmp(arr[i], arr[i + 1],
					ft_strlen(arr[i]) + ft_strlen(arr[i + 1])) > 0)
			{
				swap_strings(&arr[i], &arr[i + 1]);
				swapped = 1;
			}
			i++;
		}
	}
}

_Bool	check_key_exist(char *env, char *cmd)
{
	if (ft_strncmp(env, cmd, ft_strlen(cmd)) == 0)
		return (1);
	else if (ft_strncmp(env, cmd, ft_strlen(cmd) - 1) == 0
		&& cmd[ft_strlen(cmd) - 1] == '=')
		return (0);
	return (0);
}

_Bool	update_key(t_pipex *data, char **env, char *cmd)
{
	char	*equal;
	char	**tmp;
	int		i;
	int		flag;

	i = 0;
	flag = 0;
	tmp = env;
	while (tmp[i])
	{
		if (check_key_exist(tmp[i], cmd))
			return (1);
		equal = ft_strchr(tmp[i], '=');
		if (ft_strncmp(tmp[i], cmd, equal - tmp[i] + 1) == 0
			|| (!equal && ft_strncmp(tmp[i], cmd, ft_strlen(tmp[i]))
				== 0 && cmd[ft_strlen(tmp[i])] == '='))
		{
			flag = 1;
			free(tmp[i]);
			env[i] = ft_strdup(cmd);
			if (!env[i])
				set_error_and_print(data, -1, "strdup failed in update_key");
		}
		i++;
	}
	return (flag);
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

void	print_export(char *env, int fd_out)
{
	char		*equal;
	int			i;

	i = 0;
	equal = ft_strchr(env, '=');
	if (!equal)
		ft_printf(fd_out, "declare -x %s\n", env);
	else if (*(equal + 1) == '\0')
		ft_printf(fd_out, "declare -x %s\"\"\n", env);
	else
	{
		write(fd_out, "declare -x ", 11);
		while (env[i] != '=')
			write(fd_out, &env[i++], 1);
		i++;
		write(fd_out, "=\"", 2);
		while (env[i])
			write(fd_out, &env[i++], 1);
		write(fd_out, "\"\n", 2);
	}
}

void	do_export(t_pipex *data, char **env, char **cmd, int fd_out)
{
	int	i;

	i = 0;
	if (!cmd[1])
	{
		sort_strings(env);
		while (env[i])
			print_export(env[i++], fd_out);
	}
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
	data->env = env;
	data->exitcode = 0;
}
