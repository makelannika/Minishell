/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/05/14 16:00:31 by amakela          ###   ########.fr       */
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
		if (!line || strcmp(line, "exit") == 0) // error msg for null from readline
		{
			free(line);
				break;
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
			pipex(processes);
			free_list(&processes);
		}
	}
	return (0);
}
