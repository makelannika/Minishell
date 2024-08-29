/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/08/29 16:33:00 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	add_shlvl(t_pipex *data)
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

void	quit(t_pipex *data)
{
	ft_printf(2, "exit\n");
	free_env(data);
	exit(0);
}

void	readline_loop(t_pipex *data, t_node **processes)
{
	char	*line;

	while (1)
	{
		line = readline("MOOshell: ");
		add_history(line);
		check_sigint(data);
		if (!line)
			quit(data);
		if (!*line || input_validation(data, line) != 0)
		{
			free(line);
			continue ;
		}
		parse_input(line, processes);
		free(line);
		if (!*processes)
			exit(free_env(data));
		else if (pipex(*processes, data) == -1)
			exit(data->exitcode);
		free_parent(data);
		free_list(processes);
	}
}

int	main(void)
{
	t_node				*processes;
	t_pipex				data;

	data = (t_pipex){0};
	handle_signals(&data);
	processes = NULL;
	if (get_env(&data) == -1)
		return (-1);
	if (add_shlvl(&data) == -1)
		return (free_env(&data));
	readline_loop(&data, &processes);
	return (data.exitcode);
}
