/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:08:26 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/19 21:30:21 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <sys/stat.h>

int is_directory(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
        return 1;
    }
    return 0;
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
	
    for (int i = 0; ast->command->args[i] != NULL; i++) {
        ast->command->args[i] = expand_variable(ast->command->args[i]);
    }

    if (is_directory(ast->command->args[0])) {
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

char *expand_variable(const char *arg)
{
    char *expanded_arg = ft_strdup(arg);

    if (expanded_arg[0] == '$') {
        char *var_name = expanded_arg + 1;
        char *var_value = getenv(var_name);

        if (var_value) {
            free(expanded_arg);
            expanded_arg = ft_strdup(var_value);
        }
    }
    return expanded_arg;
}

void expand_variables_in_args(char **args)
{
	int		i;
	char	*expanded_arg;

	i = 0;
	while (args[i])
	{
		if (args[i][0] == '$')
		{
			expanded_arg = expand_variable(args[i]);
			free(args[i]);
			args[i] = expanded_arg;
		}
		i++;
	}
}

int	execute_command(t_ast_node *ast, char **env, t_minishell mini)
{
	int		status;
	pid_t	pid;

	if (ast->type == AST_COMMAND)
	{
		expand_variables_in_args(ast->command->args);
		if (ft_strcmp(ast->command->args[0], "exit") == 0)
			return (ft_exit(&mini, ast->command->args), 5);
		else if (ft_strcmp(ast->command->args[0], "cd") == 0)
			return (ft_cd(ast->command->args, mini.env), 0);
		pid = fork_and_execute(ast, env, mini);
		if (pid > 0)
		{
			waitpid(pid, &status, 0);
			return (WEXITSTATUS(status));
		}
	}
	else if (ast->type == AST_PIPELINE)
		return (execute_pipeline(ast, env, mini));
	return (-1);
}
