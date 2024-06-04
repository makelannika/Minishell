/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 13:23:06 by amakela           #+#    #+#             */
/*   Updated: 2024/06/02 13:23:09 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	si_handler(int g_signum)
{
	if (g_signum == SIGINT)
	{
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
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

void	handle_signals(t_pipex *data)
{
	data->sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &data->sa, NULL);
	data->sa.sa_handler = si_handler;
	sigaction(SIGINT, &data->sa, NULL);
	carrot_char(0);
}
