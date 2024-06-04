/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 16:56:23 by amakela           #+#    #+#             */
/*   Updated: 2024/05/23 15:52:09 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*remove_quotes(char *cmd, int count)
{
	int		i;
	int		j;
	char	*new_string;
	t_flags	f;

	i = 0;
	j = 0;
	init_flags(&f);
	new_string = ft_calloc(count, sizeof(char));
	if (!new_string)
		return (NULL);
	while (cmd[i])
	{
		if (cmd[i] == '\'' && f.in_double == -1)
			f.in_single *= -1;
		else if (cmd[i] == '\"' && f.in_single == -1)
			f.in_double *= -1;
		else
			new_string[j++] = cmd[i];
		i++;
	}
	return (new_string);
}

int	count_quotes(char *string)
{
	int		i;
	int		count;
	t_flags	f;

	i = 0;
	count = 0;
	init_flags(&f);
	while (string[i])
	{
		if (string[i] == '\'' && f.in_double == -1)
		{
			count++;
			f.in_single *= -1;
		}
		else if (string[i] == '\"' && f.in_single == -1)
		{
			count++;
			f.in_double *= -1;
		}
		i++;
	}
	return (count);
}

char	*quote_remover(char *cmd)
{
	int		remove;
	int		len;

	remove = count_quotes(cmd);
	len = ft_strlen(cmd) - remove + 1;
	return (remove_quotes(cmd, len));
}

static void	space_handler(char *cmd)
{
	int		i;
	t_flags	f;

	i = 0;
	init_flags(&f);
	while (cmd[i])
	{
		if (cmd[i] == '\'')
			f.in_single *= -1;
		else if (cmd[i] == '\"')
			f.in_double *= -1;
		if (cmd[i] == ' ' && f.in_single == -1 && f.in_double == -1)
			cmd[i] = 7;
		else
			i++;
	}
}

int	parse_cmd(t_pipex *data, char **cmd)
{
	space_handler(*cmd);
	if (expand_v2(data, cmd) == -1)
		return (close_and_free(data));
	data->cmd_str = quote_remover(*cmd);
	if (!data->cmd_str)
		return (set_exitcode(data, -1));
	return (0);
}
