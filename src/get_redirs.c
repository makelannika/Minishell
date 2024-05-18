/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:23:34 by amakela           #+#    #+#             */
/*   Updated: 2024/05/18 17:56:15 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// removes each redir after it's been saved to the redir_arr
void	remove_redir(char **string, int start, int end)
{
	while (start < end)
	{
		(*string)[start] = ' ';
		start++;
	}
}

// trims spaces from a redir string
char *trim_redir(char *redir_str)
{
	int		i;
	int		j;
	int		trim;
	char	*new_redir;
	
	i = 0;
	j = 0;
	trim = 0;
	trim = counter(redir_str, ' ');
	new_redir = ft_calloc(ft_strlen(redir_str) - trim + 1, sizeof(char));
	if (!new_redir)
		return (NULL);
	i = 0;
	while (redir_str[i])
	{
		while (redir_str[i] == ' ')
			i++;
		new_redir[j++] = redir_str[i++];
	}
	free(redir_str);
	return (new_redir);
}

// returns one redirection at a time to be stored in the 2d array
static char	*get_redir(char *string)
{
	int		i;
	char	*redir_str;
	char	*tmp;
	
	i = 1;
	while (string[i])
	{
		if ((string[i] == '<' && string[i - 1] == '<')
			|| (string[i] == '>' && string[i - 1] == '>'))
			i++;
		while (string[i] == ' ')
			i++;
		while (string[i] && string[i] != ' ')
			i++;
		redir_str = ft_substr(string, 0, i);
		if (!redir_str)
			return (NULL);
		redir_str = trim_redir(redir_str); // add check
		tmp = redir_str;
		redir_str = quote_remover(redir_str); // add check
		free(tmp);	
		remove_redir(&string, 0, i);
		ft_printf(2, "redir str: %s\n", redir_str);
		return (redir_str);
	}
	return (NULL);
}

// saves all redirs to the redir_arr
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
