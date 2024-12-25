/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:08:26 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/25 14:55:09 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <sys/stat.h>

int	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
	{
		return (1);
	}
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
	int i = 0;
	while (ast->command->args[i] != NULL)
	{
		ast->command->args[i] = expand_variable(ast->command->args[i], mini.env);
		i++;
	}
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

            if (ft_strlen(key) == key_len && ft_strncmp(env->value, key, key_len) == 0)
                return delimiter + 1;
        }
        env = env->next;
    }
    return NULL;
}

char *expand_variable(const char *arg, t_env *env)
{
    if (!arg)
        return (NULL);

    char *expanded_arg = ft_strdup(arg);
    
    if (!expanded_arg)
        return (NULL);

    if (expanded_arg[0] == '$')
    {
        char *var_name = expanded_arg + 1;
        char *var_value = get_env_value(var_name, env);

        if (var_value)
        {
            free(expanded_arg);
            expanded_arg = ft_strdup(var_value);
            if (!expanded_arg)
                return (NULL);
        }
        else
        {
            free(expanded_arg);
            expanded_arg = ft_strdup("");
            if (!expanded_arg)
                return (NULL);
        }
    }
    return (expanded_arg);
}

void expand_variables_in_args(char **args, t_env *env)
{
	int		i;
	char	*expanded_arg;

	i = 0;
	while (args[i])
	{
		// printf("Current arg: '%s'\n", ast->command->args[i]);
		if (args[i][0] == '$')
		{
			expanded_arg = expand_variable(args[i], env);
			free(args[i]);
			args[i] = expanded_arg;
		}
		i++;
	}
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
		if (fork_and_execute(ast, env, mini, &status) < 0)
			return (-1);
		return (status);
	}
	else if (ast->type == AST_PIPELINE)
	{
		return (execute_pipeline(ast, env, mini));
	}
	return (-1);
}
