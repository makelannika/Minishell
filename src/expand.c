/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 14:58:46 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/16 11:35:35 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand(char *value, char **cmd, int key_len, int key_start)
{
	char	*new_str;
	int		i;
	int		k;
	int		rest_of_str;
	char	*tmp;

	new_str = ft_calloc(ft_strlen(value) + ft_strlen(*cmd) - key_len, sizeof(char));
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

char	*get_value(int key_start, int key_len, char **env, char **cmd)
{
	char	*key;
	char	*value;
	int		i;
	int		start;
	int		found;

	found = 0;
	i = 0;
	start = key_start;
	key = ft_calloc(key_len + 1, sizeof(char));
	if (!key)
		return (NULL); // set data to error
	while (i < (key_len))
		key[i++] = (*cmd)[key_start++];
	i = 0;
	while (env[i])
	{
		if (key[0] != '\0' && ft_strncmp(key, env[i], ft_strlen(key)) == 0)
		{
			found = 1;
			value = (ft_strchr(env[i], '=') + 1);
			if(!expand(value, cmd, key_len, start))
				return (NULL);
				//free and return
			break ;
		}
		i++;
	}
	if (found == 0)
	{
		if ((*cmd)[start] == '"' || (*cmd)[start] == '\'')
		{
			printf("here1\n");
			i = 0;
			char *new_str = ft_calloc(ft_strlen(*cmd), sizeof(char));
			while (i < start - 1)
			{
				new_str[i] = (*cmd)[i];
				i++;
			}
			while ((*cmd)[start])
				new_str[i++] = (*cmd)[start++];
			char *tmp = *cmd;
			*cmd = new_str;
			free (tmp);
		}
		else
		{
			printf("here2\n");
			i = 0;
			char *new_str = ft_calloc(ft_strlen(*cmd), sizeof(char));
			while (i < start - 1)
			{
				new_str[i] = (*cmd)[i];
				i++;
			}
			start = start + key_len;
			while ((*cmd)[start])
				new_str[i++] = (*cmd)[start++];
			char *tmp = *cmd;
			*cmd = new_str;
			free (tmp);
		}
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
				char *exit_code = ft_itoa(data.exitcode);
				if (!expand(exit_code, cmd, 1, i + 1))
					return (-1); //free shit
				i++;
				continue;
			}
			while ((*cmd)[i] == '$')
				i++;
			key_start = i;
			while (ft_isalpha((*cmd)[i]) || (*cmd)[i] == '_' || (*cmd)[i] == '"' || (*cmd)[i] == '\'')
				i++;
			if (!get_value(key_start, i - key_start, env, cmd))
			{
				return (-1); // free shit
				continue;
			}
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
// 	char *str = ft_strdup("echo $\"HOME\"");
// 	char *str = ft_strdup("echo $ e");
// 	// char *str = ft_strdup("echo $word");
// 	// str = "echo $ e";
// 	// str = "echo $$$$NAME";
// 	expand_that_shit (&str, env, data);
// 	printf("%s\n", str);
// 	// int i = 0;
// 	// while (env[i])
// 	// 	free(env[i++]);
// 	// free(env);
// 	// free(str);
// }


//echo $HOME
//echo $'HOME' = HOME
//echo '$HOME' = $HOME
//echo $"'HOME'" = 'HOME'
//echo $$blah '$HOME' $'BLUE' $"'SABE'" $HOME is the $PWD = $blah $HOME BLUE 'SABE' /Users/linhnguy is the /Users/linhnguy


// echo path is /Users/linhnguy/.brew/bin:/Users/linhnguy/.brew/bin:/Users/linhnguy/.brew/bin:/Users/linhnguy/.brew/bin:/Users/linhnguy/.brew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki:/Applications/Visual: