/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 14:58:46 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/10 22:55:25 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand(char *value, char **cmd, int len, int start)
{
	char	*new_str;
	int		i;
	int		rest_of_str;

	new_str = malloc(sizeof(char) * (ft_strlen(value) + ft_strlen(*cmd) - len + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < start - 1)
	{
		new_str[i] = (*cmd)[i];
		i++;
	}
	int k = 0;
	while (i <= len + start + 1)
		new_str[i++] = value[k++];
	printf("new_str is: %s\n", new_str);
	rest_of_str = start + len;
	while ((*cmd)[rest_of_str])
		new_str[i++] = (*cmd)[rest_of_str++];
	new_str[i] = '\0';
	*cmd = new_str;
	return (*cmd);
}

char	*shrink(char **cmd, int	remove)
{
	int	i = 0;
	int j = 0;
	int	len = ft_strlen(*cmd);
	char	*new_string;

	new_string = ft_calloc(len, sizeof(char));
	if (!new_string)
		return (NULL);
	while ((*cmd)[i] && i != remove)
		new_string[j++] = (*cmd)[i++];
	i++;
	while ((*cmd)[i])
		new_string[j++] = (*cmd)[i++];
	return (new_string);
}

char	*get_value(int key_start, int key_len, char **env, char **cmd)
{
	char	*key;
	char	*value;
	int		i;
	int		start;
	
	value = NULL;
	// printf("cmd is: %s\n", &cmd[0][key_start]);
	i = 0;
	start = key_start;
	key = malloc(sizeof(char) * (key_len + 1));
	if (!key)
		return (NULL);
	while (i < (key_len))
		key[i++] = (*cmd)[key_start++];
	key[i] = '\0';
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(key, env[i], ft_strlen(key)) == 0)
		{

			value = (ft_strchr(env[i], '=') + 1);
			return (expand(value, cmd, key_len, start));
		}
		else
			return(shrink(cmd, start - 1));
		i++;
	}
	free(key);
	return
}

void	expand_that_shit(char *cmd, char **env)
{
	int 	in_quotes;
	int		i;
	int		key_start;

	
	in_quotes = -1;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'')
			in_quotes *= -1;
		if (cmd[i] == '$' && in_quotes == -1)
		{
			while (cmd[i] == '$')
				i++;
			key_start = i;
			// while ((ft_isalpha(cmd[i]) == 1) || cmd[i] == '_' || cmd[i] == '\'')
			while ((cmd[i]) != ' ' && cmd[i] != '\0')
				i++;
			get_value(key_start, i - key_start, env, &cmd);
			//check free and exit
			continue;
		}
		i++;
	}
	printf("new cmd is: %s", cmd);
}
int main()
{
    char **env = malloc(4 * sizeof(char*));
    env[0] = ft_strdup("NAME=Michael");
    env[1] = ft_strdup("AGE=36");
    env[2] = ft_strdup("HAIR=black");
    env[3] = NULL;
	
	char *str = malloc(30 * sizeof(char));
	str = "echo '$NAME' What $NAME $NAME $'NAME'";
	expand_that_shit (str, env);
}


//echo $HOME
//echo $'HOME' = HOME
//echo '$HOME' = $HOME
//echo $"'HOME'" = 'HOME'
//echo $$blah '$HOME' $'BLUE' $"'SABE'" $HOME is the $PWD = $blah $HOME BLUE 'SABE' /Users/linhnguy is the /Users/linhnguy