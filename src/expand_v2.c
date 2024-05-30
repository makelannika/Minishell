#include "../include/minishell.h"

void	check_quotes(t_quote *quote, char c)
{
	int i;

	i = 0;
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

char	*get_value(char *key, t_pipex *data)
{
	int		i;
	char	*value;
	char	*key_wequal;

	// printf("key is %s\n", key);
	key_wequal = ft_strjoin(key, "=");
	if (!key_wequal)
		return (set_error_return(data, -1, "Malloc failed in get_value"));
	value = NULL;
	i = 0;
// printf("HERE\n");
	while (data->env[i])
	{
		if (key[0] != '\0' && ft_strncmp(key_wequal, data->env[i], ft_strlen(key_wequal)) == 0)
		{
			value = (ft_strchr(data->env[i], '=') + 1);
			break ;
		}
		i++;
	}
	free(key_wequal);
	return (value);
}

char	*get_key(char **cmd, t_pipex *data, int key_start)
{
	int		i;
	int		k;
	char	*key;

	k = 0;
	i = key_start;
	while(ft_isalnum((*cmd)[i]) || (*cmd)[i] == '_')
		i++;
	key = ft_substr(*cmd, key_start, i - key_start);
	if (!key)
		return(set_error_return(data, -1, "Malloc failed in get_key"));
	return(key);
}

char	*replace_key_with_value(char *value, char **cmd, int key_start, t_pipex *data)
{
	char	*new_str;
	int		i;
	int		k;
	int		key_len;

	key_len = key_start;
	while (ft_isalnum((*cmd)[key_len]) || (*cmd)[key_len] == '_')
		key_len++;
	new_str = ft_calloc(ft_strlen(value) + ft_strlen(*cmd) - key_len, sizeof(char));
	if (!new_str)
		return (set_error_return(data, -1, "Malloc failed in replace_key_with_value"));
	new_str = ft_memcpy(new_str, *cmd, key_start - 1);
	new_str = ft_memcat(new_str, value);
	new_str = ft_memcat(new_str, &cmd[0][key_len]);
	free(*cmd);
	*cmd = new_str;
	return (*cmd);
}

int	expansion(char **cmd, t_pipex *data, int key_start)
{
	char	*value;
	int		i;
	int		end_of_value;
	char	*key;

	key = get_key(cmd, data, key_start);
	if (!key)
		return (-1);
	value = get_value(key, data);
	// printf("key: %s\n", key);
	// printf("value: %s\n", value);
	if (!value)
	{
		free(key);
		return (-1);
	}
	if (!replace_key_with_value(value, cmd, key_start, data))
	{
		free(key);
		return (-1);
	}
	end_of_value = key_start - 1 + ft_strlen(value);
	// printf("cmd: %s\n", *cmd);
	// printf("end_of_value: %d\n", end_of_value);
	free(key);
	return (end_of_value);
}

int		expandable(char **cmd, t_pipex *data, int key)
{
	int end_of_value;

	if ((*cmd)[key] == '_' || ft_isalpha((*cmd)[key]))
	{
		end_of_value = expansion(cmd, data, key);
		if (end_of_value == -1)
			return (-1);
		return (end_of_value);
	}
	// else if ((*cmd)[key] == '$')
	// {
	// 	// if more $ do stuff
	// 	//else delete entire string
	// }
	// else if ((*cmd)[key] == '?' )
	// {
	// 	//turn into ascii and replace into string
	// }
	// else if ((*cmd)[key] == '\'')
	// {
	// 	// print string with $
	// }
	// else if ((*cmd)[key] == '"')
	// {
	// 	// get rid of $
	// }
	return (0);
}

int	expand_v2(t_pipex *data, char **cmd)
{
	int		i;
	int		key_start;
	t_quote	quote;
	char	*new_str;
	
	printf("cmd: %s\n", *cmd);
	quote = NONE;
	i = 0;
	if (strchr(*cmd, '$') == NULL)
		return (0);
	while ((*cmd)[i])
	{
		check_quotes(&quote, (*cmd)[i]);
		if ((*cmd)[i] == '$' && quote != SINGLE)
		{
			i = expandable(cmd, data, i + 1);
			// printf("cmd: %s\n", *cmd);
			if (i == -1)
				return (-1);
			continue ;
		}	
		i++;
	}
	return (0);
}
int main()
{
	t_pipex data;
	data = (t_pipex){0};
    char **env = malloc(4 * sizeof(char*));
    env[0] = ft_strdup("HOME=Michael");
    env[1] = ft_strdup("AGE=36");
    env[2] = ft_strdup("HAIR=black");
    env[3] = NULL;
	data.env = env;
	// char *str = ft_strdup("echo $? \"'$HOME'\" '\"$HOME\"' What $$$HOME $HOME $AGE'$ HOME'");
	// char *str = ft_strdup("echo $\"HOME\"");
	char *str = ft_strdup("echo $HOME \"$HOME\" \'\"$HOME\'\" \"'$HOME'\"");
	// char *str = ft_strdup("echo $word");
	// str = "echo $ e";
	// str = "echo $$$$NAME";
	expand_v2 (&data, &str);
	printf("%s\n", str);
	// int i = 0;
	// while (env[i])
	// 	free(env[i++]);
	// free(env);
	// free(str);
}

// echo $HOME "$HOME" \'"$HOME"\' "'$HOME'" ALL OF THIS EXPANDS