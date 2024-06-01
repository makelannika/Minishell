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

void put_env(char **env, int fd_out)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			ft_printf(fd_out, "%s\n", env[i]);
		i++;
	}
}

void put_pwd(t_pipex *data, int fd_out)
{
	char *s;

	s = getcwd(NULL, 0);
	if (!s)
		return (set_error_and_print(data, -1, "getcwd failed"));
	ft_printf(fd_out, "%s\n", s);
}

void do_cd(t_pipex *data, char *path, char**environ)
{
	int	i;
	char *oldpwd;
	char *newpwd;
	
	i = 0;
    oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (set_error_and_print(data, -1, "getcwd failed"));
	if (chdir(path) == -1)
		return (set_error_and_print(data, -1, "chdir failed"));
	while (environ[i])
	{
		if (ft_strncmp(environ[i], "PWD=", 4) == 0)
		{
			newpwd = getcwd(NULL, 0);
			if (!newpwd)
				return (set_error_and_print(data, -1, "getcwd failed"));
			free(environ[i]);
			environ[i] = ft_strjoin("PWD=", newpwd);
			if (!environ[i])
				return (set_error_and_print(data, -1, "strjoin failed"));
		}
		else if (ft_strncmp(environ[i], "OLDPWD=", 7) == 0)
		{
			free(environ[i]);
			environ[i] = ft_strjoin("OLDPWD=", oldpwd);
			if (!environ[i])
				return (set_error_and_print(data, -1, "strjoin failed"));
		}
		i++;
	}
}

void do_unset(char **env, char **cmd)
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
}

void	do_exit(char **cmd, t_pipex *data)
{
	long	code;
	my_printffd my_printf;
	
	my_printf = ft_printf;
	if (data->count == 0 && data->cmds > 1)
		return ;
	if (!cmd[1])
		exit (data->curr_exitcode);
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
}
