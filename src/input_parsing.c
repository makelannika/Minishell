/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:13:07 by amakela           #+#    #+#             */
/*   Updated: 2024/05/23 17:20:07 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

static int	builtin_check(char	*string, t_node *process)
{
	char *cmd;
	char *tmp;

	cmd = trim_cmd(string);
	if (!cmd)
		return (-1);
	tmp = cmd;
	cmd = quote_remover(cmd);
	free(tmp);
	if (!cmd)
		return (-1);
	if (is_builtin(cmd))
		process->builtin = 1;
	free(cmd);
	return (0);
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
		free_list(processes);
		return (NULL);
	}
	add_back(processes, node);
	get_redir_arr(string, node);
	node->cmd = string;
	if (builtin_check(string, node) == -1
		|| !node->redirs || !node->cmd)
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
