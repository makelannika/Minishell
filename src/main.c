/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/06/05 21:23:13 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	shlvl(t_pipex *data, char *line)
{
	int	i;
	
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], "SHLVL=", 6) == 0)
		{
			if (update_shlvl(&data->env[i]) == -1)
			{
				free(line);
				return (free_env(data));
			}
			return (0);
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
	while (1)
	{
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
		if (ft_strncmp (line, "./minishell", 12) == 0)
		{
			if (shlvl(&data, line) == -1)
				return (-1);
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
		free_parent(&data);
		free_list(&processes);
	}
	return (data.exitcode);
}
