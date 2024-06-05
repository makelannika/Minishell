/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 17:55:25 by amakela           #+#    #+#             */
/*   Updated: 2024/06/05 17:55:27 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	get_redir_len(char	*str)
{
	int		i;
	char	quote;

	i = 1;
	while (str[i])
	{
		if (str[i] == '<' || str[i] == '>')
			i++;
		while (str[i] == ' ')
			i++;
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			return (i + 1);
		}
		while (str[i] && str[i] != ' ')
			i++;
		return (i);
	}
	return (-1);
}

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
