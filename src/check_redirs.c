/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 17:10:30 by amakela           #+#    #+#             */
/*   Updated: 2024/08/29 18:13:57 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	expand_redir(t_pipex *data, char **redir)
{
	int		i;
	char	*tmp;
	char	*copy;

	i = 0;
	copy = ft_strdup(*redir);
	if (!copy)
		return (set_exitcode(data, -1));
	if (expand_v2(data, redir) == -1)
		return (set_exitcode(data, -1));
	tmp = *redir;
	*redir = quote_remover(*redir);
	free(tmp);
	while ((*redir)[i] == '<' || (*redir)[i] == '>')
		i++;
	if (!(*redir)[i] || (*redir)[i] == '/')
	{
		print_redir_err(data, *redir + i, &copy[i]);
		free(copy);
		return (-1);
	}
	free(copy);
	return (0);
}

static int	redir_out(char **redir, t_pipex *data)
{
	if (validate_redir(data, redir) == -1)
		return (-1);
	close(data->ends[1]);
	if ((*redir)[1] == '>')
		data->ends[1] = open(*redir + 2, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		data->ends[1] = open(*redir + 1, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (data->ends[1] < 0)
	{
		if (access(*redir + 1, F_OK) != 0)
			ft_printf(2, "MOOshell: %s: No such file or directory\n", *redir + 1);
		else
			ft_printf(2, "MOOshell: %s: Permission denied\n", *redir + 1);
		data->execute = 0;
		return (set_exitcode(data, 1));
	}
	return (0);
}

static int	redir_in(char **redir, t_pipex *data)
{
	if (validate_redir(data, redir) == -1)
		return (-1);
	close(data->ends[0]);
	data->ends[0] = open(*redir + 1, O_RDONLY);
	if (data->ends[0] < 0)
	{
		if (access(*redir + 1, F_OK) != 0)
			ft_printf(2, "MOOshell: %s: No such file or directory\n",
				*redir + 1);
		else
			ft_printf(2, "MOOshell: %s: Permission denied\n", *redir + 1);
		data->execute = 0;
		return (set_exitcode(data, 1));
	}
	return (0);
}

int	check_cmd(char *string)
{
	int	i;

	i = 0;
	while (string[i])
	{
		if (string[i++] != ' ')
			return (1);
	}
	return (0);
}

int	handle_redirs(t_node *process, t_pipex *data)
{
	int	i;

	i = 0;
	while (process->redirs[i])
	{
		if (ft_strncmp(process->redirs[i], "<<", 2) == 0)
		{
			i++;
			continue ;
		}
		else if (ft_strncmp(process->redirs[i], "<", 1) == 0)
		{
			if (redir_in(&process->redirs[i], data) == -1)
				return (data->exitcode);
		}
		else if (ft_strncmp(process->redirs[i], ">", 1) == 0)
		{
			if (redir_out(&process->redirs[i], data) == -1)
				return (data->exitcode);
		}
		i++;
	}
	return (data->exitcode);
}
