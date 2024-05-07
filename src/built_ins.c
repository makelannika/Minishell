/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 19:02:26 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/05 21:00:40 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void put_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			ft_printf(2, "%s\n", env[i]);
		i++;
	}
}

void put_pwd(void)
{
	char *s;

	s = getcwd(NULL, 0);
	if (!s)
		ft_printf(2, "%s\n", strerror(errno));
	ft_printf(1, "%s\n", s);
}



void do_cd(char *path, char**environ)
{
	int	i;
	char *oldpwd;
	char *newpwd;
	
    oldpwd = getcwd(NULL, 0);
	i = 0;
	if (chdir(path) == -1)
		dprintf(2, "%s\n", strerror(errno));
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
void do_unset(char **env, char *key)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0)
			remove_string(env, i);
		i++;
	}
}
