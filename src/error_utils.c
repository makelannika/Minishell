/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 11:48:53 by linhnguy          #+#    #+#             */
/*   Updated: 2024/06/07 14:06:19 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	set_error_and_print(t_pipex *data, int error, char *msg)
{
	data->exitcode = error;
	ft_printf(2, "%s\n", msg);
}

void	print_error_and_exit(t_pipex *data, char *cmd1,
	int exitcode, int option)
{
	if (option == 1)
	{
		ft_printf(2, "exit\n");
		close_and_free(data);
		exit(exitcode);
	}
	else if (option == 2)
	{
		ft_printf(2, "exit\n");
		ft_printf(2, "MOOshell: exit: %s: numeric argument required\n", cmd1);
		close_and_free(data);
		exit(exitcode);
	}
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

void	handle_two_args(t_pipex *data, char *cmd1)
{
	long	code;

	code = 0;
	if (ft_isdigit_str(cmd1))
	{
		code = ft_atol(cmd1);
		if (code < 0)
			print_error_and_exit(data, cmd1, 255, 2);
		else if (code > 255)
			print_error_and_exit(data, NULL, code % 256, 1);
		else
			print_error_and_exit(data, NULL, code, 1);
	}
	else if (!ft_isdigit_str(cmd1))
		print_error_and_exit(data, cmd1, 255, 2);
}
