/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:10:30 by amakela           #+#    #+#             */
/*   Updated: 2024/05/17 17:04:58 by amakela          ###   ########.fr       */
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
		data->error = true;
		return (-1);
	}
	return (0);
}

static void	do_heredoc(char *file, t_pipex *data)
{
	char	*delimiter;
	char	*line;
	int		len;
	int		heredoc;
	
	delimiter = NULL;
	line = NULL;
	len = 0;
	heredoc = open(".heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	close(heredoc);
	heredoc = open(".heredoc", O_CREAT | O_RDWR | O_APPEND, 0644);
	data->ends[0] = open(".heredoc", O_RDONLY);
	delimiter = trim_cmd(&file[2]);
	len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (ft_strncmp(line, delimiter, len) == 0)
		{
			free(line);
			break ;
		}
		else
		{
			ft_printf(heredoc, "%s\n", line);
			free(line);
		}
	}
	close(heredoc);
	free(delimiter);
}

// checks rights to a redir file with '<'
static int	redir_in(char *file, t_pipex *data)
{
	if (data->ends[0] != -1)
		close(data->ends[0]);
	if (file[1] == '<') // check if open fails
		do_heredoc(file, data);
	else
	{
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
		if (ft_strncmp(processes->redirs[i], "<", 1) == 0)
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
