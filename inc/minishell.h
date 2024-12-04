/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:50:57 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/04 14:43:49 by ysetiawa         ###   ########.fr       */
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
#include <linux/limits.h> 
#include "../libft/libft.h"

#define SUCCESS 0
#define ERROR 1

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define BUFF_SIZE 4096

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
    AST_WORD,
    AST_ECHO, 
    AST_PWD, 
    AST_EXTERNAL
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
    t_ast_node *next;
    char *file;                // file name
    int type;                  // redirect in or out
} t_ast_redirect;

typedef struct s_env
{
    char *value;            // key value
    struct s_env *next;     // linkedlist
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

// exec test
int execute_command(t_ast_node *ast);

// builtins

//  echo.c
int ft_strcmp(const char *s1, const char *s2);
int number_of_args(char **args);
int ft_echo(char **args);
// env.c
int ft_env(t_env *env);
// pwd.c
int ft_pwd(void);

#endif