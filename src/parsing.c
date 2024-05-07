/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:27:03 by amakela           #+#    #+#             */
/*   Updated: 2024/05/07 13:05:26 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// for checking enclosed quotes
int	count_quotes(char *string)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (string[i])
	{
		if (string[i] == '\'' || string[i] == '\"')
			count++;
		i++;
	}
	return (count);
}

// returns the number of character c outside of quotes and single quotes in a string
int	counter(char *string, char c)
{
	int	i;
	int	count;
	int	in_quotes;
	int	in_double_quotes;
	
	i = 0;
	count = 0;
	in_quotes = -1;
	in_double_quotes = -1;
	while (string[i])
	{
		if (string[i] == '\'')
			in_quotes *= -1;
		else if (string[i] == '\"')
			in_double_quotes *= -1;
		if (string[i] == c && (in_quotes == -1 & in_double_quotes == -1))
			count++;
		i++;
	}
	return (count);
}

// parses one process at a time
static t_node	*parse_process(char	*string, t_node **processes)
{
	int	i;
	int	j;
	t_node	*node;
	
	i = 0;
	j = 0;
	node = create_node();
	if(!node)
	{
		free(string);
		return (NULL);
	}
	add_back(processes, node);
	get_redirs(string, node);
	remove_redirs(string);
	node->cmd = string;
	if (!node->redirs || !node->cmd)
	{
		free(string);
		free_list(processes);
		return (NULL);
	}
	return (node);
}

// parses the complete line returned from readline
t_node	**parse_input(char *line, t_node **processes)
{
	int	i;
	int	start;
	int	in_quotes;
	int	in_double_quotes;

	i = 0;
	start = 0;
	in_quotes = -1;
	in_double_quotes = -1;
	while (line[i])
	{
		if (line[i] == '\'')
			in_quotes *= -1;
		else if (line[i] == '\"')
			in_double_quotes *= -1;
		if (line[i] == '|' && in_quotes == -1 && in_double_quotes == -1)
		{
			if (!parse_process(ft_substr(line, start, i - start), processes))
				return (NULL);
			start = i + 1;
		}
		i++;
	}
	if (!parse_process(ft_substr(line, start, i - start), processes))
		return (NULL);
	return (processes);
}
