/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:23:34 by amakela           #+#    #+#             */
/*   Updated: 2024/06/04 13:54:34 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	remove_redir(char **string, int start, int end)
{
	while (start < end)
	{
		(*string)[start] = ' ';
		start++;
	}
}

char	*trim_redir(char *redir_str)
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
		while (redir_str[i] == ' ')
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
		redir_str = trim_redir(redir_str);
		tmp = redir_str;
		redir_str = quote_remover(redir_str);
		free(tmp);
		remove_redir(&string, 0, i);
		return (redir_str);
	}
	return (NULL);
}

int	get_redirs(char *string, t_node *node)
{
	int		i;
	int		j;
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
		if (string[i] == '<' && string[i + 1] == '>')
			i++;
		else if ((string[i] == '<' || string[i] == '>')
			&& (f.in_single == -1 & f.in_double == -1))
		{
			node->redirs[j++] = get_redir(&string[i]);
			if (!node->redirs[j - 1])
				return (-1);
			if ((string[i] == '<' && string[i + 1] == '<')
				|| (string[i] == '>' && string[i + 1] == '>'))
				i++;
		}
		i++;
	}
	return (1);
}

int	count_redirs(char *string)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (string[i])
	{
		if (string[i] == '<')
		{
			if (string[i + 1] != '>')
				count++;
			if (string[i + 1] == '<' || string[i + 1] == '>')
				i++;
		}
		else if (string[i] == '>')
		{
			count++;
			if (string[i + 1] == '>')
				i ++;
		}
		i++;
	}
	return (count);
}

void	get_redir_arr(char *string, t_node *node)
{
	int	count;

	count = count_redirs(string);
	node->redirs = ft_calloc(count + 1, sizeof(char *));
	if (get_redirs(string, node) == -1)
	{
		free_str_array(node->redirs);
		node->redirs = NULL;
	}
	if (!node->redirs)
		return ;
}
