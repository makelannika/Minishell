/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 19:02:26 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/02 18:47:19 by linhnguy         ###   ########.fr       */
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

// TODO: #include <sys/stat.h>
void do_cd(char *path)
{
	if (chdir(path) == -1)
		ft_printf(2, "%s\n", strerror(errno));
	
	//FIXME: change the home directory in the env array too
	// TODO: mkdir("test", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

static void	remove_string(char **src, int index)
{
	free(src[index]);
	while (src[index + 1])
	{
		src[index] = src[index + 1];
		index++;
	}
	src[index] = NULL;
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
