/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:23:34 by amakela           #+#    #+#             */
/*   Updated: 2024/05/09 20:30:21 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// removes redirection symbols and file names from the string
void	remove_redirs(char	*string)
{
	int i;
	t_flags	f;

	i = 0;
	init_flags(&f);
	while(string[i])
	{
		if (string[i] == '\'')
			f.in_single *= -1;
		else if (string[i] == '\"')
			f.in_double *= -1;
		if ((string[i] == '<' || string[i] == '>') && (f.in_single == -1 && f.in_double == -1))
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
		if (string[i] == ' ' || string[i] == '>')
			i++;
		while (string[i] && string[i] != ' ')
			i++;
		redir_str = ft_substr(string, 0, i);
		return (redir_str);
	}
	return (NULL);
}

int	get_redirs(char *string, t_node *node)
{
	int	i;
	int	j;
	t_flags	f;

	i = 0;
	j = 0;
	init_flags(&f);
	while (string[i])
	{
		if (string[i] == '\'')
			f.in_single *= -1;
		else if (string[i] == '\"')
			f.in_double *= -1;
		if ((string[i] == '<' || string[i] == '>') && (f.in_single == -1 & f.in_double == -1))
		{
			node->redirs[j++] = get_redir(&string[i]); 
			if (!node->redirs[j - 1])
				return (-1);
			if ((string[i] == '<' && string[i + 1] == '<') || (string[i] == '>' && string[i + 1] == '>'))
				i++;
		}
		i++;
	}
	return (1);
}

// makes a 2d array to store all redirections of a single process
void	get_redir_arr(char	*string, t_node *node)
{
	int	count;
	
	count = counter(string, '<') + counter(string, '>');
	node->redirs = ft_calloc(count + 1, sizeof(char *));
	if (!get_redirs(string, node))
		free_str_array(node->redirs);
	if (!node->redirs)
		return ;
}
