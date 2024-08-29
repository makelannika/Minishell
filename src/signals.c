/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 13:23:06 by amakela           #+#    #+#             */
/*   Updated: 2024/08/29 16:41:44 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_signum = 0;

void	si_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signum = 1;
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	ignore_signals(t_pipex *data)
{
	data->sa.sa_handler = SIG_IGN;
	data->sa.sa_flags = 0;
	sigaction(SIGQUIT, &data->sa, NULL);
	sigaction(SIGINT, &data->sa, NULL);
}

static void	carrot_char(int on)
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

void	handle_signals(t_pipex *data)
{
	data->sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &data->sa, NULL);
	data->sa.sa_handler = si_handler;
	sigaction(SIGINT, &data->sa, NULL);
	carrot_char(0);
}
