/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:23:34 by amakela           #+#    #+#             */
/*   Updated: 2024/05/22 11:14:40 by amakela          ###   ########.fr       */
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
	char	*new_redir;
	
	j = 0;
	i = counter(redir_str, ' ');
	new_redir = ft_calloc(ft_strlen(redir_str) - i + 1, sizeof(char));
	if (!new_redir)
		return (NULL);
	i = 0;
	while (redir_str[i] == '<' || redir_str[i] == '>')
		new_redir[j++] = redir_str[i++];
	if (redir_str[i] == ' ')
	{
		while(redir_str[i] == ' ')
			i++;
	}
	while (redir_str[i])
	new_redir[j++] = redir_str[i++];
	free(redir_str);
	return (new_redir);
}

int	get_redir_len(char	*str)
{
	int		i;
	char	quote;

	i = 0;
	quote = str[i];
	i++;
	while (str[i] && str[i] != quote)
		i++;
	return (i + 1);
}

int	check_syntax_error(t_pipex *data, char *string)
{
	if (string[1] == '\0')
	{
		ft_printf(2, "syntax error near unexpected token `newline'\n");
		return (set_exitcode(data, 258));
	}
	if ((string[0] == '<' && string[1] == '>')
		|| (string[0] == '>' && string[1] == '<'))
	{
		ft_printf(2, "syntax error near unexpected token `%c'\n", string[0]);
		return (set_exitcode(data, 258));
	}
	return (0);
}

// returns one redirection at a time to be stored in the 2d array
static char	*get_redir(t_pipex *data, char *string)
{
	int		i;
	char	*redir_str;
	char	*tmp;
	
	i = 1;
	while (string[i])
	{
		if (check_syntax_error(data, string) == -1)
			return (NULL);
		if ((string[i] == '<' && string[i - 1] == '<')
			|| (string[i] == '>' && string[i - 1] == '>'))
			i++;
		while (string[i] == ' ')
			i++;
		if (string[i] == '\'' || string[i] == '\"')
			i += get_redir_len(&string[i]);
		else
		{
			while (string[i] && string[i] != ' ')
				i++;
		}
		redir_str = ft_substr(string, 0, i);
		if (!redir_str)
			return (NULL);
		redir_str = trim_redir(redir_str); // add check
		tmp = redir_str;
		redir_str = quote_remover(redir_str); // add check
		free(tmp);	
		remove_redir(&string, 0, i);
		return (redir_str);
	}
	return (NULL);
}

// saves all redirs to the redir_arr
int	get_redirs(t_pipex *data, char *string, t_node *node)
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
			node->redirs[j++] = get_redir(data, &string[i]); 
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
void	get_redir_arr(t_pipex *data, char *string, t_node *node)
{
	int	count;
	
	count = counter(string, '<') + counter(string, '>');
	node->redirs = ft_calloc(count + 1, sizeof(char *));
	if (get_redirs(data, string, node) == -1)
	{
		free_str_array(node->redirs);
		node->redirs = NULL;
	}
	if (!node->redirs)
		return ;
}
