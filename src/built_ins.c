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

void put_env(t_pipex *data, char **env, int fd_out)
{
	(void)data;
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
	(void)data;
	char *s;

	s = getcwd(NULL, 0);
	if (!s)
		ft_printf(2, "%s\n", strerror(errno));
	ft_printf(fd_out, "%s\n", s);
}

void do_cd(t_pipex *data, char *path, char**environ)
{
	(void)data;
	int	i;
	char *oldpwd;
	char *newpwd;
	
    oldpwd = getcwd(NULL, 0);
	i = 0;
	if (chdir(path) == -1)
		ft_printf(2, "%s\n", strerror(errno));
	while (environ[i])
	{
		if (ft_strncmp(environ[i], "PWD=", 4) == 0)
		{
			newpwd = getcwd(NULL, 0);
			free(environ[i]);
			environ[i] = ft_strjoin("PWD=", newpwd);
			// if (!environ[i])
			// 	FREE AND PRINT FT
		}
		else if (ft_strncmp(environ[i], "OLDPWD=", 7) == 0)
		{
			free(environ[i]);
			environ[i] = ft_strjoin("OLDPWD=", oldpwd);
			// if (!environ[i])
			// 	FREE FREE AND PRINT FT
		}
		i++;
	}
}

// FIXME: unset $(env | awk -F= '{print $1}')
// FIXME: take multiple variables
// FIXME: 
void do_unset(t_pipex *data, char **env, char *key)
{
	(void)data;
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0)
			remove_string(env, i);
		i++;
	}
}

void	do_exit(char **cmd, t_pipex *data)
{
	(void)data;
	long	code;
	
	if (data->count == 0 && data->cmds > 1)
		return ;
	if (!cmd[1])
		exit (data->exitcode);
	if (cmd[1])
	{
		if (ft_isdigit_str(cmd[1]))
		{
			code = ft_atol(cmd[1]);
			if (!ft_isdigit_str(cmd[1]) || code < 0)
			{
				ft_printf(2, "%s %s: numeric argument required\n", cmd[0], cmd[1]);
				exit (255);
			}
			else if (code > 255)
				exit (code % 256);
			else
				exit (code);
		}
		else
		{
			ft_printf(2, "%s %s: command not found\n", cmd[0], cmd[1]);
			exit (127);
		}
	}
}
