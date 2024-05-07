/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 19:04:40 by amakela           #+#    #+#             */
/*   Updated: 2024/05/07 13:35:58 by amakela          ###   ########.fr       */
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
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include "../libft/libft.h"

typedef	struct node
{
	char		**redirs;
	char		*cmd;
	struct node	*next;
} t_node;

typedef struct s_pipex
{
	int		cmds;
	int		count;
	int		ends[2];
	int		read_end;
	char	**paths;
	char	*new_cmd;
	char	**cmd;
	char	*path;
	int		*pids;
	bool	error;
	int		exitcode;
}	t_pipex;

/*********************************--PARSING--**************************************/

t_node	**parse_input(char *line, t_node **processes);
t_node	*create_node();
int		count_quotes(char *string);
void	add_back(t_node **lst, t_node *new);
int		get_list_length(t_node *processes);
int		counter(char *string, char c);
void	get_redirs(char	*string, t_node *node);
void	remove_redirs(char	*string);

/**********************************--PIPEX--***************************************/

int		pipex(t_node *processes);
int		get_fds(t_pipex *data, t_node *processes);
void	handle_redirs(t_node *processes, t_pipex *data);
int		forking(t_pipex *data, t_node *processes);
void	parse_cmd(t_pipex *data, char *cmd);

/********************************--CLEANING--***************************************/
int		close_and_free(t_pipex *data);
void	free_list(t_node **processes);
void	free_str_array(char **array);

/*********************************--BUILT_IN--*************************************/

void	put_env(char **env);
void	put_pwd(void);
void	do_cd(char *path, char **env);
void	do_unset(char **env, char *key);
void	sort_strings(char **arr);
void    putstr_in_array(char ***env, char *cmd);

/**********************************--LIST_FT--*************************************/

// void	free_list(t_node **processes);
// void	free_str_array(char **array);

/***********************************--UTILS--**************************************/

int		array_len(char **a);
void	remove_string(char **src, int index);

#endif
