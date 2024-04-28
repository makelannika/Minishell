/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 19:04:40 by amakela           #+#    #+#             */
/*   Updated: 2024/04/28 17:54:44 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <term.h>
#include <termios.h>
#include <unistd.h>
#include <curses.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

typedef	struct node
{
	char		**redirs;
	char		*cmd;
	int			fd_in;
	int			fd_out;
	struct node	*next;
} t_node;


void	free_list(t_node **processes);
void	free_node(t_node *node);
void	free_str_array(char **array);

#endif
