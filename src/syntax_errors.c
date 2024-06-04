/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 17:24:13 by amakela           #+#    #+#             */
/*   Updated: 2024/06/03 17:24:15 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	quote_check(char *line)
{
	if (count_quotes(line) % 2 != 0)
	{
		ft_printf(2, "MOOshell: error: enclosed quotes\n");
		return (-1);
	}
	return (0);
}

int	check_beginning(t_pipex *data, char *line)
{
	if (line[0] == '|')
	{
		ft_printf(2, "MOOshell: syntax error near unexpected token `|'\n");
		return (set_exitcode(data, 258));
	}
	return (0);
}

int	check_pipes(t_pipex *data, char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ')
		i++;
	if (line[i] == '|')
	{
		ft_printf(2, "MOOshell: syntax error near unexpected token `|'\n");
		return (set_exitcode(data, 258));
	}
	return (0);
}

int	check_in(t_pipex *data, char *line)
{
	if (ft_strncmp(line, ">>", 2) == 0 || ft_strncmp(line, " >>", 3) == 0
		|| ft_strncmp(line, "<>>", 3) == 0 || ft_strncmp(line, "< >>", 4) == 0)
	{
		ft_printf(2, "MOOshell: syntax error near unexpected token `>>'\n");
		return (set_exitcode(data, 258));
	}
	else if (ft_strncmp(line, " <<", 3) == 0 || ft_strncmp(line, "<<<", 3) == 0)
	{
		ft_printf(2, "MOOshell: syntax error near unexpected token `<<'\n");
		return (set_exitcode(data, 258));
	}
	else if (ft_strncmp(line, ">", 1) == 0 || ft_strncmp(line, " >", 2) == 0
		|| ft_strncmp(line, "< >", 3) == 0)
	{
		ft_printf(2, "MOOshell: syntax error near unexpected token `>'\n");
		return (set_exitcode(data, 258));
	}
	else if (ft_strncmp(line, " <", 2) == 0 || ft_strncmp(line, "<<", 2) == 0
		|| ft_strncmp(line, "< <", 3) == 0)
	{
		ft_printf(2, "MOOshell: syntax error near unexpected token `<'\n");
		return (set_exitcode(data, 258));
	}
	return (0);
}

int	check_out(t_pipex *data, char *line)
{
	if (ft_strncmp(line, ">>>", 3) == 0 || ft_strncmp(line, " >>", 3) == 0)
	{
		ft_printf(2, "MOOshell: syntax error near unexpected token `>>'\n");
		return (set_exitcode(data, 258));
	}
	else if (ft_strncmp(line, "<<", 2) == 0 || ft_strncmp(line, " <<", 3) == 0
		|| ft_strncmp(line, "><<", 3) == 0 || ft_strncmp(line, "> <<", 4) == 0)
	{
		ft_printf(2, "MOOshell: syntax error near unexpected token `<<'\n");
		return (set_exitcode(data, 258));
	}
	else if (ft_strncmp(line, ">>", 2) == 0 || ft_strncmp(line, " >", 2) == 0
		|| ft_strncmp(line, "> >", 3) == 0)
	{
		ft_printf(2, "MOOshell: syntax error near unexpected token `>'\n");
		return (set_exitcode(data, 258));
	}
	else if (ft_strncmp(line, "<", 1) == 0 || ft_strncmp(line, " <", 2) == 0
		|| ft_strncmp(line, "> <", 3) == 0)
	{
		ft_printf(2, "MOOshell: syntax error near unexpected token `<'\n");
		return (set_exitcode(data, 258));
	}
	return (0);
}

int	check_line(t_pipex *data, char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '<')
		{
			if (check_in(data, &line[i + 1]) == -1)
				return (-1);
		}
		if (line[i] == '>')
		{
			if (check_out(data, &line[i + 1]) == -1)
				return (-1);
		}
		if (line[i] == '|')
		{
			if (check_pipes(data, &line[i + 1]) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}

int	check_ending(t_pipex *data, char *line)
{
	int	len;

	len = ft_strlen(line);
	if (line[len - 1] == '|' || line[len - 1] == '<' 
		|| (line[len - 1] == '>'))
	{
		ft_printf(2, "MOOshell: syntax error near unexpected token `newline'\n");
		return (set_exitcode(data, 258));
	}
	return (0);
}

int	check_syntax_error(t_pipex *data, char *line)
{
	int	i;

	i = 0;
	if (quote_check(line) == -1)
		return (-1);
	if (line[0] == '|')
	{
		ft_printf(2, "MOOshell: syntax error near unexpected token `|'\n");
		return (set_exitcode(data, 258));
	}
	if (check_line(data, line) == -1)
		return (data->exitcode);
	if (check_ending(data, line) == -1)
		return (data->exitcode);
	return (0);
}
