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
	int		key_end_index;

	key_end_index = key_start;
	while (ft_isalnum((*cmd)[key_end_index]) || (*cmd)[key_end_index] == '_'
		|| (*cmd)[key_end_index] == '?')
		key_end_index++;
	new_str = ft_calloc(ft_strlen(value) + ft_strlen(*cmd) -
		(key_end_index - key_start), sizeof(char));
	if (!new_str)
		return (set_error_return(data, -1, "Malloc failed in replace_key_with_value"));
	new_str = ft_memcpy(new_str, *cmd, key_start - 1);
	new_str = ft_memcat(new_str, value);
	new_str = ft_memcat(new_str, &cmd[0][key_end_index]);
	free(*cmd);
	*cmd = new_str;
	return (*cmd);
}

void	remove_str(char **cmd, int start, int end)
{
	int i;

	i = 0;
	while ((*cmd)[end + i])
	{
		(*cmd)[start + i] = (*cmd)[end + i];
		i++;
	}
	(*cmd)[start + i] = '\0';
}

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
		remove_str(cmd, key_start - 1, key_start + ft_strlen(key));
		return (key_start - 1);
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

void remove_dollar_sign(char **cmd, int dollar, int amount)
{
    int i;
	
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
	if(replace_key_with_value(exit_code, cmd, key, data) == NULL)
	{
		free(exit_code);
		return (-1);
	}
	return (key);
}


void	remove_not_expandable(char **cmd, int key_start)
{
	int i;

	i = 0;
	while ((*cmd)[key_start + i])
	{
		if((*cmd)[key_start + i] == '$' || (*cmd)[key_start + i] == ' ')
			remove_str(cmd, key_start, key_start - i);
		i++;
	}
	(*cmd)[key_start + i] = '\0';
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
	else if ((*cmd)[key] == '"')
		remove_dollar_sign(cmd, key - 1, 1);
	else if ((*cmd)[key] == '?' )
	{
		if (expand_exit_code(cmd, data, key) == -1)
			return (-1);
	}
	else if ((*cmd)[key] == '$')
	{
		remove_dollar_sign(cmd, key - 1, 2);
		key = key - 1;
	}
	// else
	// 	remove_not_expandable(cmd, key);
	// printf("cmd: %s\n", *cmd);
	// printf("key: %d\n", key);
	return (key);
}

int	expand_v2(t_pipex *data, char **cmd)
{
	int		i;
	t_quote	quote;
	
	printf("first cmd: %s\n", *cmd);
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
			if (i == -1)
				return (-1);
			continue ;
		}	
		i++;
	}
	printf("cmd: %s\n", *cmd);
	return (0);
}
// int main()
// {
// 	t_pipex data;
// 	data = (t_pipex){0};
//     char **env = malloc(4 * sizeof(char*));
//     env[0] = ft_strdup("HOME=Mi");
//     env[1] = ft_strdup("AGE=36");
//     env[2] = ft_strdup("HAIR=black");
//     env[3] = NULL;
// 	data.env = env;
// 	// char *str = ft_strdup("echo $? \"'$HOME'\" '\"$HOME\"' What $$$HOME $HOME $AGE'$ HOME'");
// 	// char *str = ft_strdup("echo $\"HOME\"");
// 	char *str = ft_strdup("echo $wer \"$'HOME'\"");
// 	// char *str = ft_strdup("echo $word");
// 	// str = "echo $ e";
// 	// str = "echo $$$$NAME";
// 	expand_v2 (&data, &str);
// 	printf("%s\n", str);
// 	// int i = 0;
// 	// while (env[i])
// 	// 	free(env[i++]);
// 	// free(env);
// 	// free(str);
// }

// echo $HOME "$HOME" \'"$HOME"\' "'$HOME'" ALL OF THIS EXPANDS