#include "minishell.h"

void	check_quotes(t_quote *quote, char c)
{
	int i;

	i = 0;
	if (c == '\'' || c == '"')
	{
		if (quote == NONE)
		{
			if (c == '\'')
				quote = SINGLE;
			else
				quote = DOUBLE;
		}
		else if (c == '\'' && quote == SINGLE)	
			quote = NONE;
		else if (c == '"' && quote == DOUBLE)
			quote = NONE;
	}	
}

char	*get_value(char *key, t_pipex *data)
{
	int		i;
	char	*value;
	char	*key_wequal;

	key_wequal = ft_strjoin(key, "=");
	if (!key_wequal)
		return (set_error_return(data, -1, "Malloc failed in get_value"));
	value = NULL;
	i = 0;
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
	key = ft_calloc(i - key_start, sizeof(char));
	if (!key)
		return(set_error_return(data, -1, "Malloc failed in get_key"));
	while (k < i - key_start)
		key[k++] = (*cmd)[key_start++];
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
	while ((*cmd)[key_start + key_len])
		new_str[i++] = (*cmd)[key_start + key_len++];
	free(*cmd);
	*cmd = new_str;
	return (*cmd);
}

int	expandsion(char **cmd, t_pipex *data, int key_start)
{
	char	*value;
	int		i;
	char	*key;

	key = get_key(cmd, data, key_start);
	if (!key)
		return (-1);
	value = get_value(key, data);
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
	free(key);
	return (key_start + ft_strlen(value));
}

int		expandable(char **cmd, t_pipex *data, int key)
{
	int end_of_value;

	if ((*cmd)[key] == '_' || ft_isalpha((*cmd)[key]))
	{
		//get_value and replace value into string 
		end_of_value = expandsion(cmd, data, key);
		if (end_of_value == -1)
		//free
			return (-1);
		return (end_of_value);
	}
	else if ((*cmd)[key] == '$')
	{
		// if more $ do stuff
		//else delete entire string
	}
	else if ((*cmd)[key] == '?' )
	{
		//turn into ascii and replace into string
	}
	else if ((*cmd)[key] == '\'')
	{
		// print string with $
	}
	else if ((*cmd)[key] == '"')
	{
		// get rid of $
	}
	return (0);
}

int	expand_v2(t_pipex *data, char **cmd)
{
	int		i;
	int		key_start;
	t_quote	quote;
	char	*new_str;
	
	quote = NONE;
	i = 0;
	if (strchr(*cmd, '$') == NULL)
		return (0);
	while ((*cmd)[i])
	{
		check_quotes(&quote, (*cmd)[i]);
		if ((*cmd)[i] == '$' && quote != SINGLE)
		{
			if(expandable(cmd, data, i + 1) == -1)
				return (-1);
		}	
		i++;
	}
	return (0);
}