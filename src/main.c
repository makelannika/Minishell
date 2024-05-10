/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/05/09 19:28:06 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// test main for parsing
int	main()
{
	char	*line;
	t_node	*processes = NULL;
	extern	char **environ;
	
	line = readline("minishell: ");
	if (count_quotes(line) % 2 != 0)
		ft_printf(2, "Error\nUnclosed quotes\n");
	parse_input(line, &processes);
	free(line);
	if (!processes)
		return (-1);
	pipex(processes);
	free_list(&processes);
	return (0);
}