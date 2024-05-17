/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:10:30 by amakela           #+#    #+#             */
/*   Updated: 2024/05/17 14:48:14 by linhnguy         ###   ########.fr       */
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
		data->error = 1;
		return (-1);
	}
	return (0);
}

// checks rights to a redir file with '<'
static int	redir_in(char *file, t_pipex *data)
{
	if (data->ends[0] != -1)
		close(data->ends[0]);
	// if (file[1] == '<')
		// heredoc
	data->ends[0] = open(&file[1], O_RDONLY);
	if (data->ends[0] < 0)
	{
		if (access(&file[1], F_OK) != 0)
			ft_printf(2, "no such file or directory: %s\n", &file[1]);
		else
			ft_printf(2, "2permission denied: %s\n", &file[1]);
		data->error = 1;
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
