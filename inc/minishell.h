/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:50:57 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/11 20:34:38 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/wait.h>

# define SUCCESS 0
# define ERROR 1

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define BUFF_SIZE 4096
# define INITIAL_SIZE 4096

// token types
typedef enum
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	APPEND
}							t_token_type;

// token struct
typedef struct s_token
{
	t_token_type type;    // token type
	char *value;          // token value
	struct s_token *next; // next token
}							t_token;

// ast node types
typedef enum
{
	AST_PIPELINE,
	AST_COMMAND,
	AST_REDIRECT,
	AST_WORD
}							t_ast_node_type;

// ast node struct
typedef struct s_ast_node
{
	t_ast_node_type			type;
	struct s_ast_pipeline	*pipeline;
	struct s_ast_command	*command;
	struct s_ast_redirect	*redirect;
	char					*word;

}							t_ast_node;

typedef struct s_ast_command
{
	char **args;          // commands
	t_ast_node *redirect; // redirect node (if there is)
	char					*heredoc;
}							t_ast_command;

typedef struct s_ast_pipeline
{
	t_ast_node *left;  // left side of the pipe
	t_ast_node *right; // right side of the pipe
}							t_ast_pipeline;

typedef struct s_ast_redirect
{
	t_ast_node				*next;
	char *file; // file name
	int type;   // redirect in or out
}							t_ast_redirect;

typedef struct s_env
{
	char *value;        // key value
	struct s_env *next; // linkedlist
}							t_env;

typedef struct s_minishell
{
	t_env					*env;
	int						exit;
}							t_minishell;

// token
t_token						*create_token(t_token_type type, const char *value);
void						add_token(t_token **head, t_token *new_token);
t_token						*lexer(const char *input);
void						free_tokens(t_token *tokens);
void						print_tokens(t_token *tokens);

// ast
t_ast_node					*parse_command(t_token **tokens);
t_ast_node					*parse_pipeline(t_token **tokens);
t_ast_node					*parse_redirect(t_token **tokens);
t_ast_node					*build_ast(t_token *tokens);
void						free_ast(t_ast_node *node);
void						print_ast(t_ast_node *node, int depth);

// exec
int							execute_command(t_ast_node *ast, char **env, t_minishell mini);
char						*find_executable(const char *cmd);
char						*concat_path(const char *dir, const char *cmd);
char						*read_heredoc(const char *delim);

// builtins

//  echo.c
int							ft_strcmp(const char *s1, const char *s2);
int							number_of_args(char **args);
int							ft_echo(char **args);
char						*remove_quotes(const char *str);
int							is_valid_n_flag(const char *arg);
// env.c
int							ft_env(t_env *env);
int							env_init(t_minishell *mini, char **env);
// pwd.c
int							ft_pwd(void);
// free.c
void						free_env(t_env *env);
// exit.c
int							check_is_number(char *str);
void						ft_exit(t_minishell *mini, char **av);
// cd.c
void						print_cd_error(const char *path);
char						*find_env_variable(t_env *env_list,
								const char *variable, size_t len);
int							update_previous_directory(t_env *env_list);
int							navigate_to_special_directory(int option,
								t_env *env_list);
int							ft_cd(char **arguments, t_env *env_list);
int							handle_tilde(char **path, t_env *env_list);

#endif
