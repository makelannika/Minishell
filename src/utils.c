/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:38:41 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/17 20:40:37 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	array_len(char **a)
{
	int i;

	i = 0;
	while(a[i])
		i++;
	return (i);
}

void	remove_string(char **env, int index)
{
	free(env[index]);
	while (env[index + 1])
	{
		env[index] = env[index + 1];
		index++;
	}
	env[index] = NULL;
}

_Bool	ft_isdigit_str(char *str)
{
	while (*str)
	{
		if (*str < '0' && *str > '9')
			return (0);
		str++;
	}
	return (1);
}

void	set_error_and_print(t_pipex *data, int error, char *msg)
{
	data->exitcode = error;
	ft_printf(2, "%s\n", msg);
}

void	print_error_and_exit(my_printffd my_printf, char *cmd0, char *cmd1, int exitcode) //use only for exit builtin
{
	if	(my_printf && cmd0 && cmd1)
		my_printf(2, "%s %s: numeric argument required\n", cmd0, cmd1);
	exit(exitcode);
}

void	free_and_exit(t_pipex *data, int exitcode) //use only for exit builtin
{
	close_and_free(data);
	exit(exitcode);
}