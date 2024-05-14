/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:13:07 by amakela           #+#    #+#             */
/*   Updated: 2024/05/11 17:52:37 by amakela          ###   ########.fr       */
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
	t_flags	f;
	
	i = 0;
	count = 0;
	init_flags(&f);
	while (string[i])
	{
		if (string[i] == '\'')
			f.in_single *= -1;
		else if (string[i] == '\"')
			f.in_double *= -1;
		if (string[i] == c && (f.in_single == -1 && f.in_double == -1))
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
	get_redir_arr(string, node);
	node->cmd = string;
	if (!node->redirs || !node->cmd)
	{
		free(string);
		free_list(processes);
		return (NULL);
	}
	return (node);
}

void	init_flags(t_flags *f)
{
	f->in_single = -1;
	f->in_double = -1;
}

// parses the complete line returned from readline
t_node	**parse_input(char *line, t_node **processes)
{
	int	i;
	int	start;
	t_flags	f;

	i = 0;
	start = 0;
	init_flags(&f);
	while (line[i])
	{
		if (line[i] == '\'')
			f.in_single *= -1;
		else if (line[i] == '\"')
			f.in_double *= -1;
		if (line[i] == '|' && f.in_single == -1 && f.in_double == -1)
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
