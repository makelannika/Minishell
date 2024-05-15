/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 14:58:46 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/11 18:10:37 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#include "../include/minishell.h"

char	*expand(char *value, char **cmd, int key_len, int key_start)
{
	char	*new_str;
	int		i;
	int		k;
	int		rest_of_str;
	char	*tmp;

	new_str = ft_calloc(ft_strlen(value) + ft_strlen(*cmd) - key_len + 1, sizeof(char));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < key_start - 1)
	{
		new_str[i] = (*cmd)[i];
		i++;
	}
	k = 0;
	while (value[k])
		new_str[i++] = value[k++];
	if (k < key_len)
		key_len = k;
	rest_of_str = key_start + key_len;
	while ((*cmd)[rest_of_str])
		new_str[i++] = (*cmd)[rest_of_str++];
	tmp = *cmd;
	*cmd = new_str;
	free (tmp);
	return (*cmd);
}

char	*shrink(char **cmd, int	remove)
{
	int	i = 0;
	int j = 0;
	int	len = ft_strlen(*cmd);
	char	*new_string;
	char	*tmp;

	new_string = ft_calloc(len, sizeof(char));
	if (!new_string)
		return (NULL);
	while ((*cmd)[i] && i != remove)
		new_string[j++] = (*cmd)[i++];
	i++;
	while ((*cmd)[i])
		new_string[j++] = (*cmd)[i++];
	tmp = *cmd;
	*cmd = new_string; 
	free (tmp);
	return (*cmd);
}

char	*get_value(int key_start, int key_len, char **env, char **cmd)
{
	char	*key;
	char	*value;
	int		i;
	int		start;
	
	i = 0;
	start = key_start;
	key = ft_calloc(key_len + 1, sizeof(char));
	if (!key)
		return (NULL);
	while (i < (key_len))
		key[i++] = (*cmd)[key_start++];
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(key, env[i], ft_strlen(key)) == 0)
		{
			value = (ft_strchr(env[i], '=') + 1);
			if(!expand(value, cmd, key_len, start))
				return (NULL);
				//free and return
			break ;
		}
		else
			if(!shrink(cmd, start - 1))
				return (NULL);
		i++;
	}
	free(key);
	return (*cmd);
}

int		expand_that_shit(char **cmd, char **env, t_pipex data)
{
	int		i;
	int		key_start;
	t_quote	quote;

	i = 0;
	quote = NONE;
	while ((*cmd)[i])
	{
		if ((*cmd)[i] == '\'' || (*cmd)[i] == '"')
		{
			if (quote == NONE)
			{
				if ((*cmd)[i] == '\'')
					quote = SINGLE;
				else
					quote = DOUBLE;
			}
			else if ((*cmd)[i] == '\'' && quote == SINGLE)	
				quote = NONE;
			else if ((*cmd)[i] == '"' && quote == DOUBLE)
				quote = NONE;
		}	
		if ((*cmd)[i] == '$' && quote != SINGLE)
		{
			if((*cmd)[i + 1] == '?')
			{
				data.exitcode = 10;
				char *exit_code = ft_itoa(data.exitcode);
				if (!expand(exit_code, cmd, 1, i + 1))
					return (-1); //free shit
				i++;
				continue;
			}
			while ((*cmd)[i] == '$')
				i++;
			key_start = i;
			while (ft_isalpha((*cmd)[i]) || (*cmd)[i] == '_')
				i++;
			if (!get_value(key_start, i - key_start, env, cmd))
				return (-1); // free shit
			continue;
		}
		else
			i++;
	}
	return (0);
}
// int main()
// {
// 	t_pipex data;
// 	data = (t_pipex){0};
//     char **env = malloc(4 * sizeof(char*));
//     env[0] = ft_strdup("HOME=Michael");
//     env[1] = ft_strdup("AGE=36");
//     env[2] = ft_strdup("HAIR=black");
//     env[3] = NULL;
	
// 	char *str = ft_strdup("echo $? \"'$HOME'\" '\"$HOME\"' What $$$HOME $HOME $AGE'$ HOME'");
// 	// str = "echo $'NAME'";
// 	// str = "echo $$$$NAME";
// 	expand_that_shit (&str, env, data);
// 	int i = 0;
// 	printf("%s\n", str);
// 	while (env[i])
// 		free(env[i++]);
// 	free(env);
// 	free(str);
// }


//echo $HOME
//echo $'HOME' = HOME
//echo '$HOME' = $HOME
//echo $"'HOME'" = 'HOME'
//echo $$blah '$HOME' $'BLUE' $"'SABE'" $HOME is the $PWD = $blah $HOME BLUE 'SABE' /Users/linhnguy is the /Users/linhnguy

