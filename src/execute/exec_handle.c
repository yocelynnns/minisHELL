/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:57:08 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/11 17:00:35 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	handle_redirections(t_ast_node *ast)
{
	t_ast_node	*redirect;
	int			fd;

	redirect = ast->command->redirect;
	while (redirect)
	{
		if (redirect->redirect->type == REDIRECT_IN)
			fd = open(redirect->redirect->file, O_RDONLY);
		else if (redirect->redirect->type == REDIRECT_OUT)
			fd = open(redirect->redirect->file, O_WRONLY \
			| O_CREAT | O_TRUNC, 0644);
		else if (redirect->redirect->type == APPEND)
			fd = open(redirect->redirect->file, O_WRONLY \
			| O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
		if (redirect->redirect->type == REDIRECT_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redirect = redirect->redirect->next;
	}
}

static void	handle_heredoc(t_ast_node *ast)
{
	int	pipefd[2];

	if (!ast->command->heredoc)
		return ;
	pipe(pipefd);
	write(pipefd[1], ast->command->heredoc, ft_strlen(ast->command->heredoc));
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
}

static void	execute_pipeline(t_ast_node *ast, char **env, t_minishell mini)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		execute_command(ast->pipeline->left, env, mini);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
		execute_command(ast->pipeline->right, env, mini);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

int	execute_command(t_ast_node *ast, char **env, t_minishell mini)
{
	pid_t	pid;
	int		status;
	t_env	*env_list = NULL;

	if (ast->type == AST_COMMAND)
	{
        if (ft_strcmp(ast->command->args[0], "echo") == 0)
            return ft_echo(ast->command->args);
		else if (ft_strcmp(ast->command->args[0], "pwd") == 0)
            return ft_pwd();
		else if (ft_strcmp(ast->command->args[0], "exit") == 0)
            return (ft_exit(&mini, ast->command->args), 1);
		else if (ft_strcmp(ast->command->args[0], "cd") == 0)
            return ft_cd(ast->command->args, env_list);
		else if (ft_strcmp(ast->command->args[0], "env") == 0)
            return ft_env(env_list);
		// else if (ft_strcmp(ast->command->args[0], "export") == 0)
        //     return ft_export(ast->command->args);
		pid = fork();
		if (pid == 0)
		{
			if (ast->command->redirect)
				handle_redirections(ast);
			handle_heredoc(ast);
			exec_command_or_path(ast, env);
		}
		else if (pid < 0)
		{
			perror("fork");
			return (-1);
		}
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	else if (ast->type == AST_PIPELINE)
	{
		execute_pipeline(ast, env, mini);
		return (0);
	}
	return (-1);
}