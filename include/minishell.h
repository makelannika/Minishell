/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 19:04:40 by amakela           #+#    #+#             */
/*   Updated: 2024/05/02 17:43:47 by linhnguy         ###   ########.fr       */
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
#include <errno.h>
#include "../libft/libft.h"

typedef	struct node
{
	char		**redirs;
	char		*cmd;
	int			fd_in;
	int			fd_out;
	struct node	*next;
} t_node;

/*********************************--BUILT_IN--*************************************/

void	put_env(char **env);
void	put_pwd(void);
void	do_cd(char *path);
void	do_unset(char **env, char *key);

/**********************************--LIST_FT--*************************************/

void	free_list(t_node **processes);
void	free_node(t_node *node);
void	free_str_array(char **array);

/**********************************--Utils--***************************************/

int		array_len(char **a);

#endif