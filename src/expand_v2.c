/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_v2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:36:08 by linhnguy          #+#    #+#             */
/*   Updated: 2024/06/07 16:29:28 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	expansion(char **cmd, t_pipex *data, int key_start)
{
	char	*value;
	int		end_of_value;
	char	*key;

	key = get_key(cmd, data, key_start);
	if (!key)
		return (-1);
	value = get_value(key, data);
	if (!value)
	{
		remove_key(cmd, key_start - 1, key_start + ft_strlen(key));
		free(key);
		return (key_start - 1);
	}
	if (!replace_key(value, cmd, key_start, data))
	{
		free(key);
		return (-1);
	}
	end_of_value = key_start - 1 + ft_strlen(value);
	free(key);
	return (end_of_value);
}

void	remove_dollar_sign(char **cmd, int dollar, int amount)
{
	int	i;

	i = 0;
	while ((*cmd)[dollar + amount + i])
	{
		(*cmd)[dollar + i] = (*cmd)[dollar + amount + i];
		i++;
	}
	(*cmd)[dollar + i] = '\0';
}

int	expand_exit_code(char **cmd, t_pipex *data, int key)
{
	char	*exit_code;

	exit_code = ft_itoa(data->exitcode);
	if (!exit_code)
	{
		ft_printf(2, "Malloc failed in expand_exit_code\n");
		return (-1);
	}
	if (replace_key(exit_code, cmd, key, data) == NULL)
	{
		free(exit_code);
		return (-1);
	}
	free(exit_code);
	return (key);
}

int	expandable(char **cmd, t_pipex *data, int key, t_quote quote)
{
	int	end_of_value;

	if ((*cmd)[key] == '_' || ft_isalpha((*cmd)[key]))
	{
		end_of_value = expansion(cmd, data, key);
		if (end_of_value == -1)
			return (-1);
		return (end_of_value);
	}
	else if ((*cmd)[key] == '"' || ((*cmd)[key] == '\'' && quote != DOUBLE))
	{
		remove_dollar_sign(cmd, key - 1, 1);
		return (key - 1);
	}
	else if ((*cmd)[key] == '?')
	{
		if (expand_exit_code(cmd, data, key) == -1)
			return (-1);
	}
	else if ((*cmd)[key] == '$')
	{
		remove_dollar_sign(cmd, key - 1, 2);
		key = key - 1;
	}
	return (key);
}

int	expand_v2(t_pipex *data, char **cmd)
{
	int		i;
	t_quote	quote;

	quote = NONE;
	i = 0;
	if (strchr(*cmd, '$') == NULL)
		return (0);
	while ((*cmd)[i])
	{
		check_quotes(&quote, (*cmd)[i]);
		if ((*cmd)[i] == '$' && quote != SINGLE)
		{
			i = expandable(cmd, data, i + 1, quote);
			if (i == -1)
				return (-1);
			continue ;
		}
		i++;
	}
	return (0);
}
