/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/05/24 14:33:08 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	free_first_inits(t_pipex *data)
{
	free_str_array(data->env);
	data->env = NULL;
	free_str_array(data->paths);
	data->paths = NULL;
	return (data->exitcode);
}

int	main()
{
	char				*line;
	t_node				*processes;
	t_pipex				data;
	
	processes = NULL;
	data = (t_pipex){0};
	data.sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &data.sa, NULL);
	while (1) 
	{
		if (first_inits(&data) == -1)
		return (-1);
		// ft_printf(2, "exitcode: %d\n", data.exitcode);
		line = readline("MOOshell: ");
		if (!line)
		{
			ft_printf(2, "MOOshell: error: readline failed\n");	
				return (-1);
		}
		else if (line[0] == '\0')
			continue;
		else 
		{
			processes = NULL;
			add_history(line);
			if (count_quotes(line) % 2 != 0)
			{
				ft_printf(2, "MOOshell: error: enclosed quotes\n");
				continue;
			}
			if (check_syntax_error(&data, line) != 0)
			{
				free_first_inits(&data);
				free(line);
				continue ;
			}
			parse_input(line, &processes);
			free(line);
			if (!processes)
				return (free_first_inits(&data));
			else if (pipex(processes, &data) == -1)
				return (data.exitcode);
			// ft_printf(2, "exitcode in main: %d\n", data.exitcode);
			unlink(".heredoc");
			free_list(&processes);
		}
	}
	return (data.exitcode);
}
