/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/06/06 19:10:18 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	shlvl(t_pipex *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], "SHLVL=", 6) == 0)
		{
			if (update_shlvl(&data->env[i]) == -1)
				return (free_env(data));
		}
		i++;
	}
	return (0);
}

void	check_sigint(t_pipex *data)
{
	if (g_signum == 1)
		data->exitcode = 1;
	g_signum = 0;
}

int	main(void)
{
	char				*line;
	t_node				*processes;
	t_pipex				data;
	int					i;

	i = 0;
	data = (t_pipex){0};
	handle_signals(&data);
	processes = NULL;
	if (get_env(&data) == -1)
			return (-1);
	if (shlvl(&data) == -1)
		return (free_env(&data));
	while (1)
	{
		ft_printf(2, "1 here\n");
		line = readline("MOOshell: ");
		check_sigint(&data);
		if (!line)
		{
			ft_printf(2, "exit\n");
			free_env(&data);
			return (0);
		}
		if (!*line)
		{
			free(line);
			continue ;
		}
		add_history(line);
		if (input_validation(&data, line) != 0)
		{
			free(line);
			continue ;
		}
		parse_input(line, &processes);
		free(line);
		if (!processes)
			return (free_env(&data));
		else if (pipex(processes, &data) == -1)
			return (data.exitcode);
		ft_printf(2, "before free_parent\n");
		free_parent(&data);
		free_list(&processes);
	}
	return (data.exitcode);
}
