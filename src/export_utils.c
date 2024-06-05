/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 17:33:25 by linhnguy          #+#    #+#             */
/*   Updated: 2024/06/05 17:35:07 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	just_export_cmd(char **env, int fd_out)
{
	int	i;

	i = 0;
	sort_strings(env);
	while (env[i])
	{
		print_export(env[i], fd_out);
		i++;
	}
}

void	set_exitcode_and_env(t_pipex *data, char **env)
{
	data->env = env;
	data->exitcode = 0;
}
