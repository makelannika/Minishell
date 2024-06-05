# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/24 18:22:03 by amakela           #+#    #+#              #
#    Updated: 2024/06/05 17:59:31 by linhnguy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

LIBFTDIR	=	libft
LIBFT		=	$(LIBFTDIR)/libft.a

READLINEH	=	-I ~/.brew/Cellar/readline/8.2.10/include
READLINELIB	=	-L ~/.brew/Cellar/readline/8.2.10/lib/ -lreadline
INCLUDE		=	-I /include

CFLAGS		=	-Wall -Wextra -Werror
DEBUGFLAGS	=	-Wall -Wextra -Werror -g3 -fsanitize=address
CC			=	cc

CFILES		=	src/input_parsing.c		src/freeing_utils.c		src/built_ins.c		src/echo.c			\
				src/pipex.c				src/fd_utils.c			src/check_redirs.c	src/execution.c		\
				src/cmd_parsing.c		src/list_utils.c		src/get_redirs.c	src/main.c			\
				src/string_utils.c		src/export.c			src/expand_v2.c		src/signals.c		\
				src/input_validation.c	src/heredoc.c			src/path_utils.c	src/parsing_utils.c	\
				src/builtin_utils.c		src/env_utils.c		src/error_utils.c		src/expand_utils.c	\
				src/export_utils.c

OFILES		=	$(CFILES:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(READLINEH) $(INCLUDE)

all: $(NAME)

$(NAME): $(OFILES) $(LIBFT) 
	$(CC) $(CFLAGS) $(READLINELIB) $(OFILES) $(LIBFT) -o $(NAME)
	
$(LIBFT):
	make -C $(LIBFTDIR)

debug: $(OFILES) $(LIBFT)
	$(CC) $(DEBUGFLAGS) $(READLINELIB) $(OFILES) $(LIBFT) -o $(NAME)

clean:
	@rm -f $(OFILES)
	@make -C $(LIBFTDIR) clean

fclean: clean
	@rm -f $(NAME)
	@rm -f $(LIBFT)

re: fclean all

.PHONY: all clean fclean re
