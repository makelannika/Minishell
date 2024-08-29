/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 19:04:40 by amakela           #+#    #+#             */
/*   Updated: 2024/08/29 16:34:28 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <termios.h>
# include <curses.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

extern int			g_signum;

typedef struct s_heredoc
{
	char	*dlm;
	char	*line;
	int		dlm_len;
	int		fd;
}	t_heredoc;

typedef struct s_flags
{
	int	in_single;
	int	in_double;
}	t_flags;

typedef struct node
{
	char		**redirs;
	char		*cmd;
	int			builtin;
	struct node	*next;
}	t_node;

typedef enum e_quote
{
	NONE,
	SINGLE,
	DOUBLE
}	t_quote;

typedef struct s_pipex
{
	int					cmds;
	int					count;
	int					ends[2];
	int					read_end;
	char				**env;
	char				**paths;
	char				*cmd_str;
	char				**cmd;
	char				*path;
	int					*pids;
	int					exitcode;
	int					execute;
	_Bool				builtin;
	struct sigaction	sa;
	char				*pwd;
	char				*oldpwd;
}	t_pipex;

/***************--SIGNALS--***************/

void	si_handler(int signum);
void	handle_signals(t_pipex *data);
void	ignore_signals(t_pipex *data);

/***************--PARSING--***************/

int		get_pwds(t_pipex *data);
int		input_validation(t_pipex *data, char *string);
int		quote_check(t_pipex *data, char *line);
t_node	**parse_input(char *line, t_node **processes);
t_node	*create_node(void);
int		count_quotes(char *string);
void	add_back(t_node **lst, t_node *new);
int		get_list_length(t_node *processes);
int		counter(char *string, char c);
void	get_redir_arr(char	*string, t_node *node);
int		count_redirs(char *string);
int		get_redir_len(char	*str);
char	*trim_redir(char *redir_str);
void	remove_redir(char **string, int start, int end);
void	init_flags(t_flags *f);
char	*trim_cmd(char *cmd_str);
char	*quote_remover(char *cmd);
int		get_env(t_pipex *data);
int		update_shlvl(char **env);
_Bool	is_builtin(char *cmd);

/***************--PIPEX--***************/

int		init_data(t_pipex *data, t_node *processes);
int		get_paths(t_pipex *data);
int		pipex(t_node *processes, t_pipex *data);
int		get_fds(t_pipex *data, t_node *process);
int		handle_heredocs(t_node *process, t_pipex *data);
int		handle_redirs(t_node *process, t_pipex *data);
int		validate_redir(t_pipex *data, char **redir);
int		print_redir_err(t_pipex *data, char *redir, char *copy);
int		expand_redir(t_pipex *data, char **redir);
int		do_process(t_pipex *data, t_node *process);
int		parse_cmd(t_pipex *data, char **cmd);
_Bool	is_builtin(char *cmd);
int		call_builtin(t_pipex *data, char *cmd);
int		get_path(t_pipex *data);
int		check_cmd(char *string);

/***************--CLEANING--***************/

int		free_env(t_pipex *data);
int		close_and_free(t_pipex *data);
void	free_list(t_node **processes);
void	free_str_array(char **array);
int		set_exitcode(t_pipex *data, int exitcode);
void	free_parent(t_pipex *data);

/***************--BUILT_IN--***************/

int		expand_v2(t_pipex *data, char **cmd);
void	do_export(t_pipex *data, char **env, char **cmd, int fd_out);
void	do_echo(char **cmd, int fd_out, t_pipex *data);
void	put_env(char **env, int fd_out, t_pipex *data);
void	put_pwd(t_pipex *data, int fd_out);
void	do_cd(t_pipex *data, char **path, char **env);
void	do_unset(char **env, char **key, t_pipex *data);
void	sort_strings(char **arr);
char	**putstr_in_array(t_pipex *data, char **env, char *cmd);
void	do_exit(char **cmd, t_pipex *data);
void	update_pwds(t_pipex *data, char **env, char *oldpwd);
_Bool	update_key(t_pipex *data, char **env, char *cmd);
_Bool	check_key_exist(char *env, char *cmd);

/***************--BUILT_IN_UTILS--***************/

char	*get_value(char *key, t_pipex *data);
void	*ft_memcat(void *dst, const void *src);
void	remove_string(char **env, int index);
void	just_export_cmd(char **env, int fd_out);
void	set_exitcode_and_env(t_pipex *data, char **env);
void	handle_two_args(t_pipex *data, char *cmd1);

/***************--UTILS--***************/

int		array_len(char **a);
_Bool	ft_isdigit_str(char *str);
void	set_error_and_print(t_pipex *data, int error, char *msg);
void	print_error_and_exit(t_pipex *data, char *cmd1,
			int exitcode, int option);
char	*set_error_return(t_pipex *data, int error, char *msg);
_Bool	ft_isdigit_str(char *str);
void	freeing(char **str1, char **str2);
void	swap_strings(char **a, char **b);
void	sort_strings(char **arr);

/***************--EXPAND--***************/
char	*get_key(char **cmd, t_pipex *data, int key_start);
char	*replace_key(char *value, char **cmd, int key_start,
			t_pipex *data);
void	check_quotes(t_quote *quote, char c);
void	remove_key(char **cmd, int start, int end);

#endif
