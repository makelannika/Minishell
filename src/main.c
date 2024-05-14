/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/05/13 15:54:49 by linhnguy         ###   ########.fr       */
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
		if (!line || strcmp(line, "exit") == 0)
		{
			free(line);
				break;
		}
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
