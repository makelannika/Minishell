/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/06/04 13:50:17 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	free_first_inits(t_pipex *data)
{
	if (data->env)
	{
		free_str_array(data->env);
		data->env = NULL;
	}
	if (data->paths)
	{
		free_str_array(data->paths);
		data->paths = NULL;
	}
	if (data->pwd)
	{
		free(data->pwd);
		data->pwd = NULL;
	}
	if (data->oldpwd)
	{
		free(data->oldpwd);
		data->oldpwd = NULL;
	}
	return (-1);
}

int	update_shlvl(char **env)
{
	int		shlvl;
	char	*new_shlvl;
	char	*tmp;

	shlvl = ft_atoi(*env + 6);
	shlvl++;
	new_shlvl = ft_itoa(shlvl);
	if (!new_shlvl)
		return (-1);
	tmp = ft_strjoin("SHLVL=", new_shlvl);
	if (!tmp)
	{
		free(new_shlvl);
		return (-1);
	}
	free(new_shlvl);
	free(*env);
	*env = tmp;
	return (0);
}

int	main(void)
{
	char				*line;
	t_node				*processes;
	t_pipex				data;
	int					i;

	i = 0;
	ft_bzero(&data, sizeof(data));
	// data = (t_pipex){0};
	handle_signals(&data);
	processes = NULL;
	while (1)
	{
		if (first_inits(&data) == -1)
			return (-1);
		line = readline("MOOshell: ");
		if (!line)
		{
			ft_printf(2, "exit\n");
			free_first_inits(&data);
			return (0);
		}
		if (!*line)
		{
			free(line);
			continue ;
		}
		if (ft_strncmp (line, "./minishell", 11) == 0)
		{
			while (data.env[i])
			{
				if (ft_strncmp(data.env[i], "SHLVL=", 6) == 0)
				{
					if (update_shlvl(&data.env[i]) == -1)
					{
						free(line);
						return (free_first_inits(&data));
					}
					break ;
				}
				i++;
			}
		}
		add_history(line);
		if (check_syntax_error(&data, line) != 0)
		{
			free(line);
			continue ;
		}
		parse_input(line, &processes);
		free(line);
		if (!processes)
			return (free_first_inits(&data));
		else if (pipex(processes, &data) == -1)
			return (data.exitcode);
		close_and_free(&data);
		unlink(".heredoc");
		free_list(&processes);
	}
	return (data.exitcode);
}
