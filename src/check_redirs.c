/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:10:30 by amakela           #+#    #+#             */
/*   Updated: 2024/05/23 12:32:02 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// checks rights to a redir file with '>'
// clears files with '>' but not with '>>'
static int	redir_out(char *file, t_pipex *data)
{
	int i;

	i = 1;
	if (data->ends[1] != -1)
		close(data->ends[1]);
	if (file[1] == '>')
		data->ends[1] = open(&file[2], O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		data->ends[1] = open(&file[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (data->ends[1] < 0)
	{
		if (access(&file[1], F_OK) != 0)
			ft_printf(2, "no such file or directory: %s\n", &file[1]);
		else
			ft_printf(2, "1permission denied: %s\n", &file[1]);
		data->exitcode = 1;
		data->error = true;
		return (-1);
	}
	return (0);
}

static int	do_heredoc(char *file)
{
	char	*delimiter;
	char	*line;
	int		len;
	int		heredoc;
	
	heredoc = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC | O_APPEND, 0644);
	if (heredoc == -1)
		return (-1);
	delimiter = trim_cmd(&file[2]);
	if (!delimiter)
		return (-1);
	len = ft_strlen(delimiter);
	line = readline("> ");
	while (line && ft_strncmp(line, delimiter, len) != 0)
	{
		ft_printf(heredoc, "%s\n", line);
		free(line);
		line = readline("> ");
	}
	close(heredoc);
	free(delimiter);
	if (!line)
		return (-1);
	free(line);
	return (0);
}

static int	handle_heredoc(char *file, t_pipex *data)
{
	if (data->ends[0] != -1)
		close(data->ends[0]);
	if (do_heredoc(file) == -1)
	{
		ft_printf(2, "heredoc failed\n");
		data->error = true;
		return (-1);
	}
	data->ends[0] = open(".heredoc", O_RDONLY);
	unlink(".heredoc");
	return (0);
}

// checks rights to a redir file with '<'
static int	redir_in(char *file, t_pipex *data)
{
	if (data->ends[0] != -1)
		close(data->ends[0]);
	data->ends[0] = open(&file[1], O_RDONLY);
	if (data->ends[0] < 0)
	{
		if (access(&file[1], F_OK) != 0)
			ft_printf(2, "no such file or directory: %s\n", &file[1]);
		else
			ft_printf(2, "2permission denied: %s\n", &file[1]);
		data->error = true;
		return (-1);
	}
	return (0);
}

// checks rights to all redir files and clears ones with '>'
void	handle_redirs(t_node *processes, t_pipex *data)
{
	int	i;

	i = 0;
	while (processes->redirs[i])
	{
		if (ft_strncmp(processes->redirs[i], "<<", 2) == 0)
		{
			if (handle_heredoc(processes->redirs[i], data) == -1)
				return ;
		}
		else if (ft_strncmp(processes->redirs[i], "<", 1) == 0)
		{
			if (redir_in(processes->redirs[i], data) == -1)
				return ;
		}
		else if (ft_strncmp(processes->redirs[i], ">", 1) == 0)
		{
			if (redir_out(processes->redirs[i], data) == -1)
				return ;
		}
		i++;
	}
}
