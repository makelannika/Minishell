/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/05/17 17:13:20 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main()
{
	char			*line;
	t_node			*processes = NULL;
	extern	char	**environ;
	t_pipex			data;
	int				code;
	
	while (1) 
	{
		line = readline("Minishell: ");
		if (!line)
		{
			ft_printf(2, "Error: realine failed\n");	
				return (-1);
		}
		else if (ft_strncmp(line, "exit", 4) == 0)
		{
			if (ft_strlen(line) > 4 && line[4] == ' ' && ft_isdigit_str(ft_strchr(line, ' ') + 1))
			{
				code = ft_atoi(ft_strchr(line, ' ') + 1);
				if (code > 255)
					code = code % 256;
				data.exitcode = code;
			}
			else if (line[4] == '\0')
			{
				
			}
			else
			{
				ft_printf(2, "%s: command not found\n", line);
				free(line);
				data.exitcode = 127;
			}
		}	
		else if (line[0] == '\0')
			continue;
		else 
		{
			processes = NULL;
			add_history(line);
			if (count_quotes(line) % 2 != 0)
				ft_printf(2, "Error\nEnclosed quotes\n");
			parse_input(line, &processes);
			free(line);
			if (!processes)
				return (-1);
			pipex(processes, &data);
			free_list(&processes);
		}
	}
	return (0);
}
