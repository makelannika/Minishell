/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:30 by amakela           #+#    #+#             */
/*   Updated: 2024/05/24 14:33:08 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	free_first_inits(t_pipex *data)
{
	free_str_array(data->env);
	data->env = NULL;
	free_str_array(data->paths);
	data->paths = NULL;
	return (data->exitcode);
}

void	si_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(2, "\n", 1);
		rl_redisplay();
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}
int update_shlvl(char **env)
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

void	carrot_char(int on)
{
	struct termios	term;

	term = (struct termios){0};
	tcgetattr(STDIN_FILENO, &term);
	if (!on)
		term.c_lflag &= ~ECHOCTL;
	else
		term.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int	main()
{
	char				*line;
	t_node				*processes;
	t_pipex				data;
	int					i;

	i = 0;
	data = (t_pipex){0};
	data.sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &data.sa, NULL);
	data.sa.sa_handler = si_handler;
	sigaction(SIGINT, &data.sa, NULL);
	carrot_char(0);
	processes = NULL;
	while (1) 
	{
		if (first_inits(&data) == -1)
		return (-1);
		line = readline("MOOshell: ");
		if (!line)
		{
			ft_printf(2, "exit\n");	
				return (-1);
		}
		else if (ft_strncmp (line, "./minishell", 11) == 0)
		{
			while(data.env[i])
			{
				if (ft_strncmp(data.env[i], "SHLVL=", 6) == 0)
				{
					if (update_shlvl(&data.env[i]) == -1)
					{
						free(line);
						return (free_first_inits(&data));
					}
					break;
				}
				i++;
			}
		}
		else if (line[0] == '\0')
			continue;
		else 
		{
			processes = NULL;
			add_history(line);
			if (count_quotes(line) % 2 != 0)
			{
				ft_printf(2, "MOOshell: error: enclosed quotes\n");
				continue;
			}
			if (check_syntax_error(&data, line) != 0)
			{
				free_first_inits(&data);
				free(line);
				continue ;
			}
			parse_input(line, &processes);
			free(line);
			if (!processes)
				return (free_first_inits(&data));
			else if (pipex(processes, &data) == -1)
				return (data.exitcode);
			unlink(".heredoc");
			free_list(&processes);
		}
	}
	return (data.exitcode);
}
