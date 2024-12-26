/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:08:26 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/26 17:43:49 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <sys/stat.h>

int	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
		return (1);
	return (0);
}

void	execute_in_child(t_ast_node *ast, char **env, t_minishell mini)
{
	char	*executable_path;

	if (ast->command->redirect)
		handle_all_redirections(ast);
	if (ast->command->heredoc)
		handle_heredoc(ast);
	if (handle_builtin_commands(ast, mini) == 1)
		exit(EXIT_SUCCESS);
	if (ast->command->args[0] == NULL || ast->command->args[0][0] == '\0')
		exit(EXIT_SUCCESS);
	if (is_directory(ast->command->args[0]))
	{
		printf("cd: %s: Not a command\n", ast->command->args[0]);
		exit(EXIT_FAILURE);
	}
	executable_path = get_executable_path(ast);
	if (executable_path)
	{
		execve(executable_path, ast->command->args, env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Command not found: %s\n", ast->command->args[0]);
		exit(EXIT_FAILURE);
	}
}

int	execute_left_command(t_ast_node *ast, int pipefd[2], char **env,
		t_minishell mini)
{
	pid_t	pid1;

	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		execute_command(ast->pipeline->left, env, mini);
		exit(0);
	}
	else if (pid1 < 0)
	{
		perror("fork");
		return (-1);
	}
	return (pid1);
}

int	execute_right_command(t_ast_node *ast, int pipefd[2], char **env,
		t_minishell mini)
{
	pid_t	pid2;

	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
		execute_command(ast->pipeline->right, env, mini);
		exit(0);
	}
	else if (pid2 < 0)
	{
		perror("fork");
		return (-1);
	}
	return (pid2);
}

int	execute_pipeline(t_ast_node *ast, char **env, t_minishell mini)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid1 = execute_left_command(ast, pipefd, env, mini);
	if (pid1 > 0)
	{
		waitpid(pid1, &status, 0);
		close(pipefd[1]);
	}
	pid2 = execute_right_command(ast, pipefd, env, mini);
	if (pid2 > 0)
	{
		waitpid(pid2, &status, 0);
		close(pipefd[0]);
		close(pipefd[1]);
	}
	return (0);
}

char *get_env_value(const char *key, t_env *env)
{
    if (!key || !env)
        return (NULL);

    while (env)
    {
        char *delimiter = ft_strchr(env->value, '=');
        if (delimiter)
        {
            size_t key_len = delimiter - env->value;

            if (ft_strlen(key) == key_len && \
			ft_strncmp(env->value, key, key_len) == 0)
                return delimiter + 1;
        }
        env = env->next;
    }
    return NULL;
}

void expand_variables_in_args(char **args, t_env *env)
{
    int i = 0;
    char *expanded_arg;

    while (args[i])
    {
        expanded_arg = expand_argument(args[i], env);
        if (expanded_arg)
        {
            free(args[i]);
            args[i] = expanded_arg;
        }
        i++;
    }
}

char *expand_argument(char *arg, t_env *env)
{
    char *expanded_arg;
    int j;
    char *tmp_arg;

	j = 0;
	expanded_arg = NULL;
    tmp_arg = ft_strdup(arg);
    if (!tmp_arg)
        return (NULL);
    while (tmp_arg[j] != '\0')
    {
        if (tmp_arg[j] == '$')
        {
            expanded_arg = process_variable(tmp_arg, &j, env);
            if (expanded_arg)
            {
                free(tmp_arg);
                tmp_arg = expanded_arg;
            }
            else
                break;
        }
        j++;
    }
    return (tmp_arg);
}

void init_var(char *arg, int *j, t_var_process *vars)
{
	vars->var_name = NULL;
	vars->new_expanded_arg = NULL;
	vars->prefix = ft_strndup(arg, *j);
	vars->var_value = NULL;
    vars->var_start = *j + 1;
    vars->var_length = 0;
}

char *process_variable(char *arg, int *j, t_env *env)
{
	t_var_process	vars;
	char			*value_to_concatenate;

	init_var(arg, j, &vars);
	while (arg[vars.var_start + vars.var_length] != '\0' && \
	(isalnum(arg[vars.var_start + vars.var_length]) || \
	arg[vars.var_start + vars.var_length] == '_'))
		vars.var_length++;
	vars.var_name = ft_strndup(arg + vars.var_start, vars.var_length);
	if (vars.var_name)
	{
		vars.var_value = get_env_value(vars.var_name, env);
		if (vars.var_value)
			value_to_concatenate = vars.var_value;
		else
			value_to_concatenate = "";
		vars.new_expanded_arg = concatenate_parts(vars.prefix, \
		value_to_concatenate, arg + vars.var_start + vars.var_length);
		free(vars.var_name);
	}
	else
		vars.new_expanded_arg = NULL;
	free(vars.prefix);
	*j += vars.var_length;
	return (vars.new_expanded_arg);
}

char *concatenate_parts(char *expanded_arg, char *var_value, char *remaining_arg)
{
    char *new_expanded_arg;

    new_expanded_arg = malloc(ft_strlen(expanded_arg) + ft_strlen(var_value) + ft_strlen(remaining_arg) + 1);
    if (new_expanded_arg)
    {
        ft_strncpy(new_expanded_arg, expanded_arg, strlen(expanded_arg));
        new_expanded_arg[strlen(expanded_arg)] = '\0';
        ft_strcat(new_expanded_arg, var_value);
        ft_strcat(new_expanded_arg, remaining_arg);
    }
    return new_expanded_arg;
}

int	execute_command(t_ast_node *ast, char **env, t_minishell mini)
{
	int	status;

	if (ast->type == AST_COMMAND)
	{
		expand_variables_in_args(ast->command->args, mini.env);
		if (ft_strcmp(ast->command->args[0], "exit") == 0)
			return (ft_exit(&mini, ast->command->args), 5);
		else if (ft_strcmp(ast->command->args[0], "cd") == 0)
			return (ft_cd(ast->command->args, mini.env), 0);
		else if (ft_strcmp(ast->command->args[0], "export") == 0)
		{
			if (!ast->command->redirect)
				return (ft_export(ast->command->args, &mini.env), 0);
		}
		else if (ft_strcmp(ast->command->args[0], "unset") == 0)
		{
			if (!ast->command->redirect)
				return (ft_unset(ast->command->args, &mini), 1);
		}
		if (fork_and_execute(ast, env, mini, &status) < 0)
			return (-1);
		return (status);
	}
	else if (ast->type == AST_PIPELINE)
		return (execute_pipeline(ast, env, mini));
	return (-1);
}
