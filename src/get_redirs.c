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

static char	*get_redir(char *string)
{
	int		len;
	char	*redir_str;

	len = get_redir_len(string);
	if (len == -1)
		return (NULL);
	redir_str = ft_substr(string, 0, len);
	if (!redir_str)
		return (NULL);
	redir_str = trim_redir(redir_str);
	remove_redir(&string, 0, len);
	return (redir_str);
}

int	get_redirs(char *str, t_node *node)
{
	int		i;
	int		j;
	t_flags	f;

	i = 0;
	j = 0;
	init_flags(&f);
	while (str[i])
	{
		if (str[i] == '\'')
			f.in_single *= -1;
		else if (str[i] == '\"')
			f.in_double *= -1;
		else if ((str[i] == '<' || str[i] == '>')
			&& (f.in_single == -1 && f.in_double == -1))
		{
			node->redirs[j++] = get_redir(&str[i]);
			if (!node->redirs[j - 1])
				return (-1);
			if (str[i + 1] == '<' || str[i + 1] == '>')
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
