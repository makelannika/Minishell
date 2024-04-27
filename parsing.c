/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/04/27 17:13:51 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// adds a node to the back of the process list
void	add_back(t_node **lst, t_node *new)
{
	t_node	*current;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next != NULL)
		current = current->next;
	current->next = new;
}

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

// returns the number of character c outside quotes and single quotes
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

// removes redirection symbols and file names from the string
void	remove_redirs(char	*string)
{
	int i;
	int	in_quotes;
	int	in_double_quotes;

	i = 0;
	in_quotes = -1;
	in_double_quotes = -1;
	while(string[i])
	{
		if (string[i] == '\'')
			in_quotes *= -1;
		else if (string[i] == '\"')
			in_double_quotes *= -1;
		if ((string[i] == '<' || string[i] == '>') && (in_quotes == -1 && in_double_quotes == -1))
			{
				string[i++] = ' ';
				if (string[i] == ' ')
					i++;
				while (string[i] && string[i] != ' ')
					string[i++] = ' ';
			}
		else
			i++;
	}
}

// returns one redirection at a time to be stored in the 2d array
char	*get_redir(char *string)
{
	int		i;
	char	*redir_str;
	
	i = 1;
	while (string[i])
	{
		if (string[i] == ' ')
			i++;
		while (string[i] && string[i] != ' ')
			i++;
		redir_str = ft_substr(string, 0, i);
		return (redir_str);
	}
	return (NULL);
}

// makes the 2d array to store all redirections of a process
void	get_redirs(char	*string, t_node *node)
{
	int	i;
	int	j;
	int	count;
	int	in_quotes;
	int	in_double_quotes;

	i = 0;
	j = 0;
	in_quotes = -1;
	in_double_quotes = -1;
	count = counter(string, '<') + counter(string, '>');
	if (count == 0)
		return ;
	node->redirs = malloc(sizeof(char *) * (count + 1)); // check malloc
	while (string[i])
	{
		if (string[i] == '\'')
			in_quotes *= -1;
		else if (string[i] == '\"')
			in_double_quotes *= -1;
		if ((string[i] == '<' || string[i] == '>') && (in_quotes == -1 & in_double_quotes == -1))
			node->redirs[j++] = get_redir(&string[i]); // check malloc
		i++;
	}
	node->redirs[j] = NULL;
}

// creates a node to store information of a single process
t_node	*create_node()
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return (NULL);
	new_node->redirs = NULL;
	new_node->cmd = NULL;
	new_node->fd_in = 0;
	new_node->fd_out = 0;
	new_node->next = NULL;
	return (new_node);
}

// parses one process at a time
void	parse_process(char	*string, t_node **processes)
{
	int	i;
	int	j;
	t_node	*node;
	
	i = 0;
	j = 0;
	node = create_node();
	if(!node)
	{
		// free processes
		return ;
	}
	get_redirs(string, node);
	remove_redirs(string);
	node->cmd = ft_strtrim(string, " ");
	free(string);
	add_back(processes, node);
}

// parses the complete line returned from readline
void	parse_input(char *line, t_node **processes)
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
		if (line[i] == '|' && (in_quotes == -1 & in_double_quotes == -1))
		{
			parse_process(ft_substr(line, start, i - start), processes);
			start = i + 1;
		}
		if (line[i]) 
			i++;
	}
	parse_process(ft_substr(line, start, i - start), processes);
}

// test main for parsing
int	main()
{
	char	*line;
	t_node	*processes = NULL;
	line = readline("enter a line: ");
	if (count_quotes(line) % 2 != 0)
		ft_printf(2, "Error\nUnclosed quotes\n");
	parse_input(line, &processes);
	int	i = 0;
	while (processes != NULL)
	{
		i = 0;
		while (processes->redirs[i])
			ft_printf(1, "%s\n", processes->redirs[i++]);
		ft_printf(1, "%s\n", processes->cmd);
		ft_printf(1, "\n");
		processes = processes->next;
	}
	return (0);
}
