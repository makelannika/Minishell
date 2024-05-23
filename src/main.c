/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/05/23 12:35:02 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main()
{
	char			*line;
	t_node			*processes = NULL;
	t_pipex			data;
	
	data = (t_pipex){0};
	if (first_inits(&data) == -1)
		return (-1);
	while (1) 
	{
		line = readline("MOOshell: ");
		if (!line)
		{
			ft_printf(2, "Error: 1readline failed\n");	
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
				ft_printf(2, "Error\nEnclosed quotes\n");
				continue;
			}
			parse_input(line, &processes);
			free(line);
			if (!processes)
				return (-1);
			if (pipex(processes, &data) == -1)
				return (data.exitcode);
			free_list(&processes);
		}
	}
	return (data.exitcode);
}
