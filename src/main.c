/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/05/15 14:00:03 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main()
{
	char	*line;
	t_node	*processes = NULL;
	extern	char **environ;

	while (1) 
	{
		line = readline("Minishell: ");
		if (!line) // error msg for null from readline
				return (-1);
		else if (ft_strncmp(line, "exit", 4) == 0)
		{
			if (ft_strlen(line) > 4 && line[4] == ' ') // needs to finish this
			{
				break;
			}
			else
			{
				ft_printf(2, "%s: command not found", line);
				free(line);
				return (127);
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
					// printf("HERE1\n");
			free(line);
			if (!processes)
				return (-1);
			pipex(processes);
			printf("1cmd is %s\n", processes->cmd);
			free_list(&processes);
		}
	}
	return (0);
}
