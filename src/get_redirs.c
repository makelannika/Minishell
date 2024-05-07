/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:23:34 by amakela           #+#    #+#             */
/*   Updated: 2024/05/04 17:29:14 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
static char	*get_redir(char *string)
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

// makes a 2d array to store all redirections of a single process
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
	node->redirs = malloc(sizeof(char *) * (count + 1));
	if (!node->redirs)
		return ;
	while (string[i])
	{
		if (string[i] == '\'')
			in_quotes *= -1;
		else if (string[i] == '\"')
			in_double_quotes *= -1;
		if ((string[i] == '<' || string[i] == '>') && (in_quotes == -1 & in_double_quotes == -1))
		{
			node->redirs[j++] = get_redir(&string[i]); 
			if (!node->redirs[j - 1])
			{
				free_str_array(node->redirs);
				return ;
			}
		}
		i++;
	}
	node->redirs[j] = NULL;
}
