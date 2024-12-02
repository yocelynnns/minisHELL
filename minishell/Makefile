# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: messs <messs@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/26 17:50:54 by ysetiawa          #+#    #+#              #
#    Updated: 2024/12/02 13:34:12 by messs            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME            = minishell

CC              = cc
CFLAGS          = -Wall -Werror -Wextra
LDFLAGS			= -lreadline
RM              = rm -f

SRCS            = main.c ast.c builtins/echo.c builtins/pwd.c builtins/env.c builtins/export.c

LIBFT_PATH      = ./libft
LIBFT           = $(LIBFT_PATH)/libft.a

OBJS            = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_PATH)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	make -C $(LIBFT_PATH) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re
