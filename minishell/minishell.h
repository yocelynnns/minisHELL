/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:50:57 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/01 23:02:38 by yocelynnns       ###   ########.fr       */
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
#include <limits.h>
#include "libft/libft.h"

// token types
typedef enum
{
    WORD,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    HEREDOC,
    APPEND
} t_token_type;

// token struct
typedef struct s_token
{
    t_token_type type;          // token type
    char *value;                // token value
    struct s_token *next;       // next token
} t_token;

// ast node types
typedef enum
{
    AST_PIPELINE,
    AST_COMMAND,
    AST_REDIRECT,
    AST_WORD
} t_ast_node_type;

// ast node struct
typedef struct s_ast_node
{
    t_ast_node_type type;
    struct s_ast_pipeline *pipeline;    
    struct s_ast_command *command;      
    struct s_ast_redirect *redirect;    
    char *word;                         
} t_ast_node;

typedef struct s_ast_command
{
    char **args;               // commands
    t_ast_node *redirect;      // redirect node (if there is)
} t_ast_command;

typedef struct s_ast_pipeline
{
    t_ast_node *left;          // left side of the pipe
    t_ast_node *right;         // right side of the pipe
} t_ast_pipeline;

typedef struct s_ast_redirect
{
    char *file;                // file name
    int type;                  // redirect in or out
} t_ast_redirect;

typedef struct s_env
{
    char *value;
    struct s_env *next;
} t_env;

// token
t_token *create_token(t_token_type type, const char *value);
void add_token(t_token **head, t_token *new_token);
t_token *lexer(const char *input);
void free_tokens(t_token *tokens);
void print_tokens(t_token *tokens);

// ast
t_ast_node *parse_command(t_token **tokens);
t_ast_node *parse_pipeline(t_token **tokens);
t_ast_node *parse_redirect(t_token **tokens);
t_ast_node *build_ast(t_token *tokens);
void free_ast(t_ast_node *node);
void print_ast(t_ast_node *node, int depth);

#endif