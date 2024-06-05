/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 11:48:53 by linhnguy          #+#    #+#             */
/*   Updated: 2024/06/05 21:49:48 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	set_error_and_print(t_pipex *data, int error, char *msg)
{
	data->exitcode = error;
	ft_printf(2, "%s\n", msg);
}

void	print_error_and_exit(t_my_printffd my_printf,
		char *cmd0, char *cmd1, int exitcode)
{
	if (my_printf && cmd0 && cmd1)
		my_printf(2, "%s %s: numeric argument required\n", cmd0, cmd1);
	exit(exitcode);
}

void	free_and_exit(t_pipex *data, int exitcode)
{
	close_and_free(data);
	exit(exitcode);
}

char	*set_error_return(t_pipex *data, int error, char *msg)
{
	data->exitcode = error;
	ft_printf(2, "%s\n", msg);
	return (NULL);
}

void	remove_not_expandable(char **cmd, int key_start)
{
	int	i;

	i = 0;
	while ((*cmd)[key_start + i])
	{
		if ((*cmd)[key_start + i] == '$' || (*cmd)[key_start + i] == ' ')
			remove_key(cmd, key_start, key_start - i);
		i++;
	}
	(*cmd)[key_start + i] = '\0';
}
