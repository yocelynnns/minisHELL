/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messs <messs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:50:57 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/11/29 17:00:01 by messs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>

typedef enum e_token_type
{
    WORD,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    HEREDOC,
    APPEND,
    OPEN_PAREN,
    CLOSE_PAREN,
    DOLLAR
} t_token_type;

typedef struct s_token
{
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;

typedef struct s_env
{
    char *value;
    struct s_env *next;
}t_shell;

#endif