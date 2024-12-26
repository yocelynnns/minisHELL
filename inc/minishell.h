#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
// #include <limits.h>
# include <ctype.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/wait.h>
# include <termios.h>

# define SUCCESS 0
# define ERROR 1

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define BUFF_SIZE 4096
# define INITIAL_SIZE 4096

extern int					g_exit_status;

typedef enum
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	APPEND
}							t_token_type;

typedef struct s_token
{
	t_token_type			type;
	char					*value;
	struct s_token			*next;
}							t_token;

typedef struct s_lexer_state
{
	t_token					*token_list;
	int						start;
	char					quote;
}							t_lexer_state;

typedef enum
{
	AST_PIPELINE,
	AST_COMMAND,
	AST_REDIRECT,
	AST_WORD
}							t_ast_node_type;

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
	char					**args;
	t_ast_node				*redirect;
	char					*heredoc;
}							t_ast_command;

typedef struct s_ast_pipeline
{
	t_ast_node				*left;
	t_ast_node				*right;
}							t_ast_pipeline;

typedef struct s_ast_redirect
{
	t_ast_node				*next;
	char					*file;
	int						type;
}							t_ast_redirect;

typedef struct s_env
{
	char					*value;
	struct s_env			*next;
}							t_env;

typedef struct s_minishell
{
	t_env					*env;
	int						exit;
}							t_minishell;

typedef struct s_heredoc
{
	char					*content;
	size_t					total_length;
	size_t					current_size;
	const char				*delimiter;
	size_t					delimiter_length;
}							t_heredoc;

typedef struct s_signal
{
	int						sigint;
	int						sigquit;
	int						exit_status;
	pid_t					pid;
}							t_signal;

typedef struct s_var_process
{
	char					*var_name;
	char					*new_expanded_arg;
	char					*prefix;
	char					*var_value;
	int						var_start;
	int						var_length;
}							t_var_process;

extern t_signal				g_sig;

t_token						*create_token(t_token_type type, const char *value);
void						add_token(t_token **head, t_token *new_token);
t_token						*lexer(const char *input);
void						free_tokens(t_token *tokens);
void						print_tokens(t_token *tokens);
//
void						handle_redirect_in(t_lexer_state *state,
								const char *input, int *i);
void						handle_redirect_out(t_lexer_state *state,
								const char *input, int *i);
void						handle_special_char(t_lexer_state *state,
								const char *input, int *i);
void						handle_quotes_spaces(t_lexer_state *state,
								const char *input, int *i);
void						handle_variable_expansion(t_lexer_state *state,
								const char *input, int *i);

t_ast_node					*create_ast_node(t_ast_node_type type);
t_ast_node					*parse_command(t_token **tokens);
t_ast_node					*parse_pipeline(t_token **tokens);
t_ast_node					*parse_redirect(t_token **tokens);
t_ast_node					*build_ast(t_token *tokens);
void						free_ast(t_ast_node *node);
void						print_ast(t_ast_node *node, int depth);

int							is_redirect(int type);
void						attach_redirect(t_ast_node *cmd,
								t_ast_node *redirect_node);
int							handle_redirect(t_ast_node *cmd, t_token **tokens);

int							execute_command(t_ast_node *ast, char **env,
								t_minishell mini);
char						*find_executable(char *cmd);
char						*concat_path(char *dir, char *cmd);
char						*read_heredoc(const char *delim);

int							handle_builtin_commands(t_ast_node *ast,
								t_minishell mini);
void						handle_redirection(t_ast_node *ast);
void						handle_heredoc(t_ast_node *ast);
t_heredoc					*init_heredoc(const char *delimiter);
char						*check_directory(char *dir, char *cmd);
char						*get_executable_path(t_ast_node *ast);
int							fork_and_execute(t_ast_node *ast, char **env,
								t_minishell mini, int *status);
void						execute_in_child(t_ast_node *ast, char **env,
								t_minishell mini);
int							execute_left_command(t_ast_node *ast, int pipefd[2],
								char **env, t_minishell mini);
int							execute_right_command(t_ast_node *ast,
								int pipefd[2], char **env, t_minishell mini);
int							execute_pipeline(t_ast_node *ast, char **env,
								t_minishell mini);
char						*resize_buffer(char *content, size_t total_length,
								size_t *current_size);
ssize_t						read_line(char *content, size_t total_length,
								size_t current_size);
int							is_delimiter(const char *content,
								const char *delimiter, size_t total_length,
								size_t delimiter_length);
int							read_until_delimiter(t_heredoc *hd);
void						free_dirs(char **dirs);
void						handle_all_redirections(t_ast_node *ast);

int							ft_strcmp(const char *s1, const char *s2);
int							number_of_args(char **args);
int							ft_echo(char **args);
char						*remove_quotes(const char *str);
int							is_valid_n_flag(const char *arg);

int							ft_env(t_env *env);

int							init_env_list(t_minishell *mini, char **env_array);
t_env						*create_env_node(char *value);
int							env_init(t_minishell *mini, char **env);
void						print_sorted_env(t_env *env);
void						sort_env_array(char **env_array, int count);
char						**create_env_array(int count);
int							copy_env_values(t_env *env, char **env_array);
char						**env_to_array(t_env *env, int count);
int							count_env_vars(t_env *env);

int							ft_pwd(void);

void						free_env(t_env *env);
void						free_node(t_minishell *mini, t_env *env);

int							check_is_number(char *str);
void						ft_exit(t_minishell *mini, char **av);

void						print_cd_error(const char *path);
char						*get_env_variable(t_env *env_list,
								const char *variable, size_t len);
int							update_oldpwd(t_env *env_list);
char						*get_special_directory_path(int option,
								t_env *env_list);
int							navigate_to_special_dir(int option,
								t_env *env_list);
int							handle_tilde(char **path, t_env *env_list);
int							handle_special_cd(char **arguments,
								t_env *env_list);
int							handle_regular_cd(char *path, t_env *env_list);
int							ft_cd(char **arguments, t_env *env_list);

int							print_export_error(int error, const char *arg);
int							is_valid_env(char *arg);
int							extract_key_value(char *arg, char **key,
								char **new_value);
int							update_env(char *key, char *new_value, t_env **env);
int							add_env(char *new_value, t_env **env);
int							add_or_update_env_var(char *arg, t_env **env);
int							ft_export(char **args, t_env **env);

size_t						env_size(char *env);
int							unset_env_var(t_minishell *mini, char *arg);
int							ft_unset(char **args, t_minishell *mini);

char						*ft_strcpy(char *s1, char *s2);
char						*ft_strcat(char *dest, char *src);
char						*ft_strndup(const char *s, size_t n);

void						sig_int_handler(int code);
void						sig_quit_handler(int code);
void						init_signals(void);
void						handle_eof(char *line);
char						*expand_argument(char *arg, t_env *env);
char						*process_variable(char *arg, int *j, t_env *env);
char						*concatenate_parts(char *expanded_arg,
								char *var_value, char *remaining_arg);
char						*get_env_value(const char *key, t_env *env);
char						*ft_strncpy(char *dest, char *src, unsigned int n);

#endif
