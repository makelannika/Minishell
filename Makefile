# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amakela <amakela@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/24 18:22:03 by amakela           #+#    #+#              #
#                                                                              #
# **************************************************************************** #

LIBFT		=	$(LIBFTDIR)/libft.a

READLINEH	=	-I ~/.brew/Cellar/readline/8.2.10/include
READLINELIB	=	-L ~/.brew/Cellar/readline/8.2.10/lib/ -lreadline
INCLUDE		=	-I /include
NCURSESLIB	=	-lncurses

CFLAGS		=	-Wall -Wextra -Werror
DEBUGFLAGS	=	-Wall -Wextra -Werror -g -fsanitize=address
CC			=	cc

CFILES		=	src/parsing.c		src/freeing_utils.c		src/built_ins.c		src/echo.c			\
				src/pipex.c			src/fd_utils.c			src/check_redirs.c	src/forking.c		\
				src/cmd_parsing.c	src/list_utils.c		src/get_redirs.c	src/main.c
					
OFILES		=	$(CFILES:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(READLINEH) $(INCLUDE)

all: $(NAME)

$(NAME): $(OFILES) $(LIBFT) 
	$(CC) $(CFLAGS) $(READLINELIB) $(OFILES) $(LIBFT) -o $(NAME)
	
$(LIBFT):
	make -C $(LIBFTDIR)

debug: $(OFILES) $(LIBFT)
	$(CC) $(DEBUGFLAGS) $(READLINELIB) $(OFILES) $(LIBFT) -o $(NAME)

clean:
	rm -f $(OFILES)
	make -C $(LIBFTDIR) clean

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBFT)

re: fclean all

.PHONY: all clean fclean re
