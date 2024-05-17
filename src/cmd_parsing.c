/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 16:56:23 by amakela           #+#    #+#             */
/*   Updated: 2024/05/17 20:23:18 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// copies cmd without unwanted quotes to the parsed cmd string
static void	remove_quotes(t_pipex *data, char *cmd)
{
	int i;
	int	j;
	int	in_quotes;
	int	in_double_quotes;

	i = 0;
	j = 0;
	in_quotes = -1;
	in_double_quotes = -1;
	while (cmd[i])
	{
		if (cmd[i] == '\'' && in_double_quotes == -1)
			in_quotes *= -1;
		else if (cmd[i] == '\"' && in_quotes == -1)
			in_double_quotes *= -1;
		else
			data->cmd_str[j++] = cmd[i];
		i++;
	}
}

// counts quotes that need to be removed from the cmd str
static int	count_removable_quotes(char *cmd)
{
	int	i;
	int	count;
	t_flags	f;

	i = 0;
	count = 0;
	init_flags(&f);
	while (cmd[i])
	{
		if (cmd[i] == '\'' && f.in_double == -1)
		{
			count++;
			f.in_single *= -1;
		}
		else if (cmd[i] == '\"' && f.in_single == -1)
		{
			count++;
			f.in_double *= -1;
		}
		i++;
	}
	return (count);
}

// creates a new parsed cmd str into pipex's struct 
static char	*quote_remover(t_pipex *data, char *cmd)
{
	int	remove;

	remove = count_removable_quotes(cmd);
	data->cmd_str = ft_calloc(ft_strlen(cmd) -	remove + 1, sizeof(char));
	if (!data->cmd_str)
		return (NULL);
	remove_quotes(data, cmd);
	return (data->cmd_str);
}

// changes spaces outside of quotes to unprintable characters for split
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

// handles spaces and quotes of the cmd
void	parse_cmd(t_pipex *data, char **cmd)
{
	if (!(*cmd)[0])
	{
		ft_printf(2, "3permission denied: %s\n", *cmd);
		close_and_free(data);
		// exit(126);
		return ;
	}
	space_handler(*cmd);
	expand_that_shit(cmd, data->env, *data);
	data->cmd_str = quote_remover(data, *cmd);
}
