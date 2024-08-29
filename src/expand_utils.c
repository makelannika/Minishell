/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:38:41 by linhnguy          #+#    #+#             */
/*   Updated: 2024/08/29 18:15:38 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	check_quotes(t_quote *quote, char c)
{
	if (c == '\'' || c == '"')
	{
		if (*quote == NONE)
		{
			if (c == '\'')
				*quote = SINGLE;
			else
				*quote = DOUBLE;
		}
		else if (c == '\'' && *quote == SINGLE)
			*quote = NONE;
		else if (c == '"' && *quote == DOUBLE)
			*quote = NONE;
	}
}

char	*get_key(char **cmd, t_pipex *data, int key_start)
{
	int		i;
	char	*key;

	i = key_start;
	while (ft_isalnum((*cmd)[i]) || (*cmd)[i] == '_')
		i++;
	key = ft_substr(*cmd, key_start, i - key_start);
	if (!key)
		return (set_error_return(data, -1, "Malloc failed in get_key"));
	return (key);
}

char	*replace_key(char *value, char **cmd, int key_start, t_pipex *data)
{
	char	*new_str;
	int		key_end_index;

	key_end_index = key_start;
	while (ft_isalnum((*cmd)[key_end_index]) || (*cmd)[key_end_index] == '_'
		|| (*cmd)[key_end_index] == '?')
		key_end_index++;
	new_str = ft_calloc(ft_strlen(value) + ft_strlen(*cmd)
			- (key_end_index - key_start), sizeof(char));
	if (!new_str)
		return (set_error_return(data, -1,
				"Malloc failed in replace_key"));
	new_str = ft_memcpy(new_str, *cmd, key_start - 1);
	new_str = ft_memcat(new_str, value);
	new_str = ft_memcat(new_str, &cmd[0][key_end_index]);
	free(*cmd);
	*cmd = new_str;
	return (*cmd);
}
