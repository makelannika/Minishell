/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 16:56:23 by amakela           #+#    #+#             */
/*   Updated: 2024/08/29 16:05:00 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	data->cmd = ft_split(data->cmd_str, 7);
	if (!data->cmd)
	{
		ft_printf(2, "MOOshell: error: split failed\n");
		return (set_exitcode(data, -1));
	}
	if (data->cmd[0] == NULL)
	{
		ft_printf(2, "MOOshell: %s: command not found\n", data->cmd_str);
		return (set_exitcode(data, 127));
	}
	return (0);
}
