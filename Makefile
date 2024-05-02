# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/24 18:22:03 by amakela           #+#    #+#              #
#    Updated: 2024/05/01 14:25:06 by linhnguy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

LIBFTDIR	=	libft
LIBFT		=	$(LIBFTDIR)/libft.a

READLINEH	=	-I ~/.brew/Cellar/readline/8.2.10/include
READLINELIB	=	-L ~/.brew/Cellar/readline/8.2.10/lib/ -lreadline
NCURSESLIB	=	-lncurses

CFLAGS		=	-Wall -Wextra -Werror
DEBUGFLAGS	=	-Wall -Wextra -Werror -g -fsanitize=address
CC			=	cc

CFILES		=	src/parsing.c src/freeing_utils.c src/built_in.c src/echo.c

OFILES		=	$(CFILES:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(READLINEH)

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
