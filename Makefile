# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/26 17:50:54 by ysetiawa          #+#    #+#              #
#    Updated: 2024/12/03 21:08:16 by ysetiawa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME            = minishell

INC				= inc/
SRC_DIR         = src/

CC              = cc
CFLAGS          = -Wall -Werror -Wextra -g -I$(INC)
LDFLAGS			= -lreadline
RM              = rm -f

LIBFT_PATH      = libft
LIBFT           = $(LIBFT_PATH)/libft.a

PARSING_DIR		= $(SRC_DIR)parsing/ast.c \
				  $(SRC_DIR)parsing/token.c 

BUILTINS_DIR	= $(SRC_DIR)builtins/echo.c \
				  $(SRC_DIR)builtins/pwd.c \
				  $(SRC_DIR)builtins/env.c \
				  $(SRC_DIR)builtins/export.c 

ENV_DIR			= $(SRC_DIR)env/get_env.c

UTILS_DIR		= $(SRC_DIR)utils/free.c

EXEC_DIR		= $(SRC_DIR)execute/exec.c

SRCS            = $(SRC_DIR)main.c $(SRC_DIR)print.c \
				  $(PARSING_DIR) $(BUILTINS_DIR) \
				  $(ENV_DIR) $(UTILS_DIR) $(EXEC_DIR)

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
