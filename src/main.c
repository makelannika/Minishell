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

int	main()
{
	char				*line;
	t_node				*processes;
	t_pipex				data;
	
	processes = NULL;
	data = (t_pipex){0};
	if (first_inits(&data) == -1)
		return (-1);
	while (1) 
	{
		line = readline("MOOshell: ");
		if (!line)
		{
			ft_printf(2, "Error: readline failed\n");	
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
				ft_printf(2, "Error: Enclosed quotes\n");
				continue;
			}
			parse_input(line, &processes);
			free(line);
			if (!processes)
			{
				free_str_array(data.env);
				free_str_array(data.paths);
				return (-1);
			}
			if (pipex(processes, &data) == -1)
				return (data.exitcode);
			free_list(&processes);
		}
	}
	return (data.exitcode);
}
