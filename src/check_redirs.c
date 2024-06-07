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

static int	expand_redir(t_pipex *data, char **redir)
{
	int		i;
	char	*tmp;
	char	copy[ft_strlen(*redir) + 1];

	i = 0;
	ft_strlcpy(copy, *redir, ft_strlen(*redir) + 1);
	if (expand_v2(data, redir) == -1)
		return (set_exitcode(data, -1));
	tmp = *redir;
	*redir = quote_remover(*redir);
	free(tmp);
	while ((*redir)[i] == '<' || (*redir)[i] == '>')
		i++;
	if (!(*redir)[i] || (*redir)[i] == '/')
	{
		if ((*redir)[i] == '/')
			ft_printf(2, "MOOshell: %s: Is a directory\n", *redir + i);
		else if (ft_strchr(copy,'\"'))
			ft_printf(2, "MOOshell: : No such file or directory\n");
		else
			ft_printf(2, "MOOshell: %s: ambiguous redirect\n", &copy[i]);
		data->execute = 0;
		return (-1);
	}
	return (0);
}

static int	redir_out(char **file, t_pipex *data)
{
	int		i;
	char	*tmp;

	i = 1;
	if (ft_strchr(*file, '$'))
	{
		if (expand_redir(data, file) == -1)
			return (set_exitcode(data, 1));
	}
	else
	{
		tmp = *file;
		*file = quote_remover(*file);
		free(tmp);
	}
	close(data->ends[1]);
	if ((*file)[1] == '>')
		data->ends[1] = open(*file + 2, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		data->ends[1] = open(*file + 1, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (data->ends[1] < 0)
	{
		if (access(file[1], F_OK) != 0)
			ft_printf(2, "MOOshell: %s: No such file or directory\n", file[1]);
		else
			ft_printf(2, "MOOshell: %s: Permission denied\n", file[1]);
		data->execute = 0;
		return (set_exitcode(data, 1));
	}
	return (0);
}

static int	redir_in(char **file, t_pipex *data)
{
	if (expand_redir(data, file) == -1)
		return (set_exitcode(data, 1));
	close(data->ends[0]);
	data->ends[0] = open(file[1], O_RDONLY);
	if (data->ends[0] < 0)
	{
		if (access(file[1], F_OK) != 0)
			ft_printf(2, "MOOshell: %s: No such file or directory\n", file[1]);
		else
			ft_printf(2, "MOOshell: %s: Permission denied\n", file[1]);
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


// >$USER		amakela		works
// >'$USER'		$USER		works
// >"$USER"		amakela		works
// >$'USER'		USER		works
// >$"USER"		USER		works
// >$"US"ER		USER		works
// >"$US"ER		ER			works
// >'$US'ER		$USER		works

// >$USR		$USR: ambiguous redirect			works
// >"$USR"		 : No such file or directory		works
// >'$USR'		$USR								works
// >$"USR"		USR									works		
// >$'USR'		USR									works


// >$HOME		/Users/amakela: Is a directory		works
// >"$HOME"		/Users/amakela: Is a directory		works
// >'$HOME'		$HOME								works
// >$"HOME"		HOME								works
// >$'HOME'		HOME								works

// >"HOME"		HOME								works
// >'HOME'		HOME								works
// >HO"ME"		HOME								works
