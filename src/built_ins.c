/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 19:02:26 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/22 19:00:56 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void put_env(char **env, int fd_out, t_pipex *data)
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

void put_pwd(t_pipex *data, int fd_out)
{
	char *s;

	s = getcwd(NULL, 0);
	if (!s)
		s = data->pwd;
	ft_printf(fd_out, "%s\n", s);
	// char *s;
	// int i;

	// s = NULL;
	// i = 0;
	// while(data->env[i])
	// {
	// 	if (ft_strncmp(data->env[i], "PWD=", 4) == 0)
	// 	{
	// 		s = ft_strchr(data->env[i], '=') + 1;
	// 		ft_printf(fd_out, "%s\n", s);
	// 		break ;
	// 	}
	// 	i++;
	// }
	data->exitcode = 0;
}

void do_cd(t_pipex *data, char **path, char**environ)
{
	int	i;
	char *oldpwd;
	char *newpwd;
	
	i = 0;
    oldpwd = getcwd(NULL, 0);
	if (!path[1] || ft_strncmp(path[1], "~", 2) == 0)
	{
		if (chdir(get_value("HOME", data)) == -1)
			return (set_error_and_print(data, 1, "HOME not set"));
	}
	else if (!oldpwd || chdir(path[1]) == -1)
	{
		ft_printf(2, "cd: %s: 1 No such file or directory\n", path[1]);
		data->exitcode = 1;
		return;
	}
	while (environ[i])
	{
		if (ft_strncmp(environ[i], "PWD=", 4) == 0)
		{
			newpwd = getcwd(NULL, 0);
			if (!newpwd)
				return (set_error_and_print(data, -1, "2getcwd failed"));
			free(environ[i]);
			environ[i] = ft_strjoin("PWD=", newpwd);
			free (data->pwd);
			data->pwd = ft_strdup(environ[i]);
			// printf("PWD: %s\n", environ[i]);
			if (!environ[i])
				return (set_error_and_print(data, -1, "strjoin failed"));
		}
		else if (ft_strncmp(environ[i], "OLDPWD=", 7) == 0)
		{
			free(environ[i]);
			environ[i] = ft_strjoin("OLDPWD=", oldpwd);
			free (data->oldpwd);
			data->oldpwd = ft_strdup(environ[i]);
			// printf("OLDPWD: %s\n", environ[i]);
			if (!environ[i])
				return (set_error_and_print(data, -1, "strjoin failed"));
		}
		i++;
	}
	data->exitcode = 0;
}

void do_unset(char **env, char **cmd, t_pipex *data)
{
	int	i;
	int	j;
	int len;

	i = 0;
	j = 1;
	while (cmd[j])
	{
		len = ft_strlen(cmd[j]);
		i = 0;
		while (env[i])
		{
 			if (ft_strncmp(env[i], cmd[j], len) == 0 && 
				(env[i][len] == '=' || env[i][len] == '\0'))
				remove_string(env, i);
			i++;
		}
		j++;
	}
	data->exitcode = 0;
}

void	do_exit(char **cmd, t_pipex *data)
{
	long	code;
	my_printffd my_printf;
	
	my_printf = ft_printf;
	if (data->count == 0 && data->cmds > 1)
		return ;
	if (!cmd[1])
		exit (data->exitcode);
	if (cmd[1])
	{
		if (ft_isdigit_str(cmd[1]))
		{
			code = ft_atol(cmd[1]);
			if (code < 0)
				print_error_and_exit(my_printf, cmd[0], cmd[1], 255);
			else if (code > 255)
				free_and_exit(data, code % 256);
			else
				free_and_exit(data, code);
		}
		else if (!ft_isdigit_str(cmd[1]))
			print_error_and_exit(my_printf, cmd[0], cmd[1], 255);
		else
			print_error_and_exit(my_printf, cmd[0], cmd[1], 127);
	}
	data->exitcode = 0;
}
