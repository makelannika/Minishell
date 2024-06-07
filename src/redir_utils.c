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

int	print_redir_err(t_pipex *data, char *redir, char *copy)
{
	if (*redir == '/')
		ft_printf(2, "MOOshell: %s: Is a directory\n", redir);
	else if (ft_strchr(copy, '\"'))
		ft_printf(2, "MOOshell: : No such file or directory\n");
	else
		ft_printf(2, "MOOshell: %s: ambiguous redirect\n", copy);
	data->execute = 0;
	return (-1);
}

int	validate_redir(t_pipex *data, char **redir)
{
	char	*tmp;

	if (ft_strchr(*redir, '$'))
	{
		if (expand_redir(data, redir) == -1)
			return (set_exitcode(data, 1));
	}
	else
	{
		tmp = *redir;
		*redir = quote_remover(*redir);
		free(tmp);
		if (!*redir)
			return (set_exitcode(data, -1));
	}
	return (0);
}

int	get_redir_len(char	*str)
{
	int		i;

	i = 1;
	while (str[i])
	{
		if (str[i] == '<' || str[i] == '>')
			i++;
		while (str[i] == ' ')
			i++;
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
