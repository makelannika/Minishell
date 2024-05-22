/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/05/22 13:46:36 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main()
{
	char			*line;
	t_node			*processes = NULL;
	t_pipex			data;
	
	data = (t_pipex){0};
	get_env(&data);
	while (1) 
	{
		line = readline("MOOshell: ");
		if (!line)
		{
			if (data.pids == 0)
				ft_printf(2, "in child process\n");
			ft_printf(2, "error is %i\n", data.exitcode);
			ft_printf(2, "Error: 1readline failed\n");	
				return (-1);
		}
		if (line[0] == '\0')
			continue;
		else 
		{
			processes = NULL;
			add_history(line);
			if (count_quotes(line) % 2 != 0)
			{
				ft_printf(2, "Error\nEnclosed quotes\n");
				continue;
			}
			parse_input(line, &processes);
			free(line);
			if (!processes)
				return (-1);
			pipex(processes, &data);
			if (data.pids == 0)
				return (data.exitcode);
			free_list(&processes);
		}
	}
	ft_printf(2, "exitcode of the parent: %d\n", data.exitcode);
	return (data.exitcode);
}