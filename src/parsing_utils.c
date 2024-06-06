/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 21:37:49 by amakela           #+#    #+#             */
/*   Updated: 2024/06/04 21:37:51 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	quote_check(t_pipex *data, char *line)
{
	if (count_quotes(line) % 2 != 0)
	{
		ft_printf(2, "MOOshell: error: enclosed quotes\n");
		return (set_exitcode(data, 258));
	}
	return (0);
}

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

char	*trim_cmd(char *cmd_str)
{
	int		i;
	int		len;
	int		start;

	i = 0;
	len = 0;
	start = 0;
	while (cmd_str[i] && cmd_str[i] == ' ')
		i++;
	start = i;
	while (cmd_str[i] && cmd_str[i] != ' ')
	{
		i++;
		len++;
	}
	return (ft_substr(cmd_str, start, len));
}
