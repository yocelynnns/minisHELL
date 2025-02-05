/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:22:00 by yocelynnns        #+#    #+#             */
/*   Updated: 2025/02/05 19:00:25 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	handle_builtin_commands(t_ast_node *ast, t_minishell *mini)
{
	g_sig.exit_value = 0;
	if (ft_strcmp(ast->command->args[0], "echo") == 0)
		return (ft_echo(ast->command->args, mini), 0);
	else if (ft_strcmp(ast->command->args[0], "cd") == 0)
		return (ft_cd(ast->command->args, mini->env), 0);
	else if (ft_strcmp(ast->command->args[0], "pwd") == 0)
		return (ft_pwd(), 0);
	else if (ft_strcmp(ast->command->args[0], "env") == 0)
		return (ft_env(mini->env), 0);
	else if (ft_strcmp(ast->command->args[0], "export") == 0)
		return (ft_export(ast->command->args, &mini->env), 0);
	else if (ft_strcmp(ast->command->args[0], "unset") == 0)
		return (ft_unset(ast->command->args, mini), 0);
	else if (ft_strcmp(ast->command->args[0], "exit") == 0)
		return (ft_exit(ast->command->args, mini), 0);
	g_sig.exit_value = 1;
	return (1);
}

void	handle_redirection(t_ast_node *redirect, t_minishell *mini, int *org_fd)
{
	int	fd;
	(void)mini;

	if (redirect->redirect->type == REDIRECT_IN)
		fd = open(redirect->redirect->file, O_RDONLY);
	else if (redirect->redirect->type == REDIRECT_OUT)
		fd = open(redirect->redirect->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redirect->redirect->type == APPEND)
		fd = open(redirect->redirect->file, O_WRONLY | O_CREAT | O_APPEND,
				0644);
	else
		return ;
	if (fd < 0)
	{
		perror("open");
		return ;
	}
	if (redirect->redirect->type == REDIRECT_IN)
	{
		close(org_fd[1]);
		dup2(fd, STDIN_FILENO);
	}
	else
	{
		close(org_fd[0]);
		dup2(fd, STDOUT_FILENO);
	}	
	close(fd);
}

void	handle_all_redirections(t_ast_node *ast, t_minishell *mini, int *org_fd)
{
	t_ast_node	*redirect;

	redirect = ast->command->redirect;
	while (redirect)
	{
		handle_redirection(redirect, mini, org_fd);
		redirect = redirect->redirect->next;
	}
}

void	handle_heredoc(t_ast_node *ast)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	write(pipefd[1], ast->command->heredoc, ft_strlen(ast->command->heredoc));
	close(pipefd[1]);
	dup2(pipefd[0], STDIN);
	close(pipefd[0]);
}

t_heredoc	*init_heredoc(const char *delimiter)
{
	t_heredoc	*hd;

	hd = malloc(sizeof(t_heredoc));
	if (!hd)
	{
		perror("malloc");
		return (NULL);
	}
	hd->content = malloc(INITIAL_SIZE);
	if (!hd->content)
	{
		perror("malloc");
		free(hd);
		return (NULL);
	}
	hd->total_length = 0;
	hd->current_size = INITIAL_SIZE;
	hd->delimiter = delimiter;
	hd->delimiter_length = ft_strlen(delimiter);
	return (hd);
}
