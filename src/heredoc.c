/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:46:23 by amakela           #+#    #+#             */
/*   Updated: 2024/06/04 17:46:25 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <sys/ioctl.h>

static int	g_signum = 0;

static void	si_heredoc(int sig)
{
	char	str[2];

	if (sig == SIGINT)
	{
		g_signum = 1;
		str[0] = 4;
		str[1] = 0;
		ioctl(STDIN_FILENO, TIOCSTI, str);
	}
}

int	init_heredoc(t_heredoc *h, char *file)
{
	h->fd = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC | O_APPEND, 0644);
	if (h->fd == -1)
		return (-1);
	h->dlm = ft_strdup(&file[2]);
	if (!h->dlm)
	{
		close(h->fd);
		return (-1);
	}
	h->dlm_len = ft_strlen(h->dlm);
	return (0);
}

static int	do_heredoc(char *file, t_pipex *data)
{
	t_heredoc	h;

	if (init_heredoc(&h, file) == -1)
		return (-1);
	data->sa.sa_handler = si_heredoc;
	sigaction(SIGINT, &data->sa, NULL);
	while (g_signum != 1)
	{
		h.line = readline("> ");
		if (!h.line || ft_strncmp(h.line, h.dlm, h.dlm_len + 1) == 0)
			break ;
		ft_printf(h.fd, "%s\n", h.line);
		free(h.line);
	}
	if (!h.line && g_signum != 1)
		write(1, "\033[1A\033[2C", 9);
	close(h.fd);
	free(h.dlm);
	free(h.line);
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

int	handle_heredocs(t_node *process, t_pipex *data)
{
	int	i;

	i = 0;
	while (process->redirs[i])
	{
		if (ft_strncmp(process->redirs[i], "<<", 2) == 0)
		{
			if (handle_heredoc(process->redirs[i], data) == -1)
				return (data->exitcode);
			if (g_signum == 1)
			{
				data->execute = 0;
				g_signum = 0;
				return (0);
			}
		}
		i++;
	}
	unlink(".heredoc");
	return (0);
}
