/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 19:04:40 by amakela           #+#    #+#             */
/*   Updated: 2024/05/18 18:11:36 by amakela          ###   ########.fr       */
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
#include <fcntl.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include "../libft/libft.h"

typedef struct s_flags
{
	int	in_single;
	int	in_double;
} 	t_flags;

typedef	struct node
{
	char		**redirs;
	char		*cmd;
	struct node	*next;
} 	t_node;

typedef enum e_quote
{
	NONE,
	SINGLE,
	DOUBLE
}	t_quote;

typedef struct s_pipex
{
	int		cmds;
	int		count;
	int		ends[2];
	int		read_end;
	char	**env;
	char	**paths;
	char	*cmd_str;
	char	**cmd;
	char	*path;
	int		*pids;
	_Bool	error;
	int		exitcode;
	_Bool	builtin;
	struct sigaction *sa;
}	t_pipex;

/*********************************--PARSING--**************************************/

t_node	**parse_input(char *line, t_node **processes);
t_node	*create_node();
int		count_quotes(char *string);
void	add_back(t_node **lst, t_node *new);
int		get_list_length(t_node *processes);
int		counter(char *string, char c);
void	get_redir_arr(char	*string, t_node *node);
void	init_flags(t_flags *f);
int		expand_that_shit(char **cmd, char **env, t_pipex data);
char	*trim_cmd(char *cmd_str);
char	*quote_remover(char *cmd);

/**********************************--PIPEX--***************************************/

int		init_data(t_pipex *data, t_node *processes);
int		pipex(t_node *processes, t_pipex *data);
int		get_fds(t_pipex *data, t_node *processes);
void	handle_redirs(t_node *processes, t_pipex *data);
int		forking(t_pipex *data, t_node *processes);
int		parse_cmd(t_pipex *data, char **cmd);

/********************************--CLEANING--***************************************/
int		close_and_free(t_pipex *data);
void	free_list(t_node **processes);
void	free_str_array(char **array);

/*********************************--BUILT_IN--*************************************/

char    **do_export(char **env, char **cmd, int fd_out);
void	do_echo(char **cmd, int fd_out);
void	put_env(char **env, int fd_out);
void	put_pwd(int	fd_out);
void	do_cd(char *path, char **env);
void	do_unset(char **env, char *key);
void	sort_strings(char **arr);
char	**putstr_in_array(char **env, char *cmd);

/***********************************--UTILS--**************************************/

int		array_len(char **a);
void	remove_string(char **src, int index);
_Bool	ft_isdigit_str(char *str);

#endif
