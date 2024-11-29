# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/26 17:50:54 by ysetiawa          #+#    #+#              #
#    Updated: 2024/11/26 20:10:59 by ysetiawa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME            = minishell

CC              = cc
CFLAGS          = -Wall -Werror -Wextra
LDFLAGS			= -lreadline
RM              = rm -f

SRCS            = main.c

OBJS            = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re