/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 16:56:23 by amakela           #+#    #+#             */
/*   Updated: 2024/05/04 17:05:51 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// removes quotes and single quotes from the cmd string
static char	*quote_remover(t_pipex *data, char *cmd)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	j = 0;
	count = 0;
	while (cmd[i++])
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
			count++;
	}
	data->new_cmd = malloc(sizeof(char) * (ft_strlen(cmd) - count + 1));
	if (!data->new_cmd)
		return (NULL);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
			i++;
		data->new_cmd[j++] = cmd[i++];
	}
	data->new_cmd[j] = '\0';
	return (data->new_cmd);
}

// changes spaces outside of single quotes to unprintable characters
static void	space_handler(char *cmd)
{
	int		i;
	int		in_quotes;

	i = 0;
	in_quotes = -1;
	while (cmd[i])
	{
		if (cmd[i] == '\'')
			in_quotes *= -1;
		if (cmd[i] == ' ' && in_quotes == -1)
			cmd[i] = 7;
		else
			i++;
	}
}

void	parse_cmd(t_pipex *data, char *cmd)
{
	if (!cmd[0])
	{
		ft_printf(2, "permission denied: %s\n", cmd);
		close_and_free(data);
		exit(126); // nope
	}
	space_handler(cmd);
	// expand here
	data->new_cmd = quote_remover(data, cmd);
}
