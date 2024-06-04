/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:10:30 by amakela           #+#    #+#             */
/*   Updated: 2024/05/24 15:52:26 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <sys/ioctl.h>

static int signum = 0;

// checks rights to a redir file with '>'
// clears files with '>' but not with '>>'
static int	redir_out(char *file, t_pipex *data)
{
	int i;

	i = 1;
	close(data->ends[1]);
	if (file[1] == '>')
		data->ends[1] = open(&file[2], O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		data->ends[1] = open(&file[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (data->ends[1] < 0)
	{
		if (access(&file[1], F_OK) != 0)
			ft_printf(2, "MOOshell: no such file or directory: %s\n", &file[1]);
		else
			ft_printf(2, "MOOshell: permission denied: %s\n", &file[1]);
		data->execute = 0;
		return (set_exitcode(data, 1));
	}
	return (0);
}

void	si_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		char	str[2];
		signum = 1;
		str[0] = 4;
		str[1] = 0;
		ioctl(STDIN_FILENO, TIOCSTI, str);
	}
}

static int	do_heredoc(char *file, t_pipex *data)
{
	char	*delimiter;
	char	*line;
	int		len;
	int		heredoc;

	heredoc = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC | O_APPEND, 0644);
	if (heredoc == -1)
		return (-1);
	delimiter = ft_strdup(&file[2]);
	if (!delimiter)
		return (-1);
	len = ft_strlen(delimiter);
	data->sa.sa_handler = si_heredoc;
	sigaction(SIGINT, &data->sa, NULL);
	while (signum != 1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, len + 1) == 0)
			break ;
		ft_printf(heredoc, "%s\n", line);
		free(line);
	}
	close(heredoc);
	free(delimiter);
	if (!line && signum != 1)
		write(1, "\033[1A\033[2C", 9);
	free(line);
	return (0);
}

static int	handle_heredoc(char *file, t_pipex *data)
{
	close(data->ends[0]);
	if (do_heredoc(file, data) == -1)
	{
		ft_printf(2, "MOOshell: heredoc failed\n");
		return (set_exitcode(data, -1));
	}
	data->ends[0] = open(".heredoc", O_RDONLY);
	if (data->ends[0] == -1)
	{
		ft_printf(2, "MOOshell: heredoc failed\n");
		return (set_exitcode(data, -1));
	}
	return (0);
}

// checks rights to a redir file with '<'
static int	redir_in(char *file, t_pipex *data)
{
	close(data->ends[0]);
	data->ends[0] = open(&file[1], O_RDONLY);
	if (data->ends[0] < 0)
	{
		if (access(&file[1], F_OK) != 0)
			ft_printf(2, "MOOshell: no such file or directory: %s\n", &file[1]);
		else
			ft_printf(2, "MOOshell: permission denied: %s\n", &file[1]);
		data->execute = 0;
		return (set_exitcode(data, 1));
	}
	return (0);
}

int	is_empty(char *string)
{
	int	i;

	i = 0;
	while (string[i])
	{
		if (string[i++] != ' ')
			return (0);
	}
	return (1);
}

int	handle_heredocs(t_node *process, t_pipex *data)
{
	int	i;
	int heredoc;

	i = 0;
	heredoc = 0;
	data->execute = 1;
	while (process->redirs[i])
	{
		if (ft_strncmp(process->redirs[i], "<<", 2) == 0)
		{
			heredoc = 1;
			if (handle_heredoc(process->redirs[i], data) == -1)
				return (data->exitcode);
			if (signum == 1)
			{
				data->execute = 0;
				signum = 0;
				return (0);
			}
		}
		i++;
	}
	if (heredoc && is_empty(process->cmd))
		data->execute = 0;
	return (0);
}

// checks rights to all redir files and clears ones with '>'
int	handle_redirs(t_node *process, t_pipex *data)
{
	int	i;

	i = 0;
	{
		if (handle_heredocs(process, data) == -1)
			return (-1);
	}
	while (process->redirs[i])
	{
		if (ft_strncmp(process->redirs[i], "<<", 2) == 0)
		{
			i++;
			continue ;
		}
		else if (ft_strncmp(process->redirs[i], "<", 1) == 0)
		{
			if (redir_in(process->redirs[i], data) == 1)
				return (data->exitcode);
		}
		else if (ft_strncmp(process->redirs[i], ">", 1) == 0)
		{
			if (redir_out(process->redirs[i], data) == 1)
				return (data->exitcode);
		}
		i++;
	}
	return (data->exitcode);
}
