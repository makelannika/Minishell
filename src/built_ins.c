/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 19:02:26 by linhnguy          #+#    #+#             */
/*   Updated: 2024/06/06 23:11:00 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	put_env(char **env, int fd_out, t_pipex *data)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			ft_printf(fd_out, "%s\n", env[i]);
		i++;
	}
	data->exitcode = 0;
}

void	put_pwd(t_pipex *data, int fd_out)
{
	char	*s;

	s = getcwd(NULL, 0);
	if (!s)
		s = data->pwd;
	ft_printf(fd_out, "%s\n", s);
	free (s);
	data->exitcode = 0;
}

void	do_cd(t_pipex *data, char **path, char**env)
{
	int		i;
	char	*oldpwd;

	i = 0;
	oldpwd = getcwd(NULL, 0);
	if (!path[1] || ft_strncmp(path[1], "~", 2) == 0)
	{
		if (chdir(get_value("HOME", data)) == -1)
			return (set_error_and_print(data, 1, "HOME not set"));
	}
	else if (!oldpwd || chdir(path[1]) == -1)
	{
		ft_printf(2, "cd: %s: No such file or directory\n", path[1]);
		data->exitcode = 1;
		return ;
	}
	while (env[i])
		update_pwds(data, &env[i++], oldpwd);
	data->exitcode = 0;
}

void	do_unset(char **env, char **cmd, t_pipex *data)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 1;
	while (cmd[j])
	{
		len = ft_strlen(cmd[j]);
		i = 0;
		while (env[i])
		{
			if (ft_strncmp(env[i], cmd[j], len) == 0 && (env[i][len] == '='
				|| env[i][len] == '\0'))
				remove_string(env, i);
			i++;
		}
		j++;
	}
	data->exitcode = 0;
}

void	do_exit(char **cmd, t_pipex *data)
{
	if (!cmd[1])
		print_error_and_exit(data, cmd[0], 0, 1);
	else if (cmd[2])
	{
		if (ft_isdigit_str(cmd[1]))
		{
			ft_printf(2, "exit\nMOOshell: exit: too many arguments\n");
			data->exitcode = 1;
			return ;
		}
		else
			print_error_and_exit(data, cmd[1], 255, 2);
	}
	else if (cmd[1])
		handle_two_args(data, cmd[1]);
	data->exitcode = 0;
}
