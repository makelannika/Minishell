/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/05/17 21:33:42 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main()
{
	char			*line;
	t_node			*processes = NULL;
	extern	char	**environ;
	t_pipex			data;
	long			code;
	
	data = (t_pipex){0};
	while (1) 
	{
		line = readline("MOOshell: ");
		if (!line)
		{
			ft_printf(2, "Error: realine failed\n");	
				return (-1);
		}
		else if (ft_strncmp(line, "exit", 4) == 0)
		{
			if (ft_strlen(line) > 4 && line[4] == ' ')
			{
				if (!ft_isdigit_str(ft_strchr(line, ' ') + 1))
				{
					ft_printf(2, "%s: numeric argument required\n", line);
					free(line);
					return (255);
				}
				code = ft_atol(ft_strchr(line, ' ') + 1);
				if (code < 0)
				{
					ft_printf(2, "%s: numeric argument required\n", line);
					return (255);
				}
				if (code > 255)
					code = code % 256;
				return (code);
			}
			else if (line[4] == '\0')
			{
				free(line);
				return (data.exitcode);
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
