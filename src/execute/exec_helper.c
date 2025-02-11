/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 15:35:01 by hthant            #+#    #+#             */
/*   Updated: 2025/02/04 21:10:05 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	execute_left_command(t_cmd *m, int pipefd[2], char **env,
		t_minishell *mini)
{
	pid_t	pid1;

	// (void) m;
	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		if (ft_strcmp(mini->ast->pipeline->left->command->args[0], "exit") == 0)
		{
			close(m->org_fd[0]);
			close(m->org_fd[1]);
		}
		execute_command(mini->ast->pipeline->left, env, mini);
		// printf("-----------------dfadsf\n");
		// cleanup(mini);
		// exit(0);
	}
	else if (pid1 < 0)
	{
		perror("fork");
		return (-1);
	}
	return (pid1);
}

int	execute_right_command(t_cmd *m, int pipefd[2], char **env,
		t_minishell *mini)
{
	pid_t	pid2;

	// (void) m;
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
		close(m->org_fd[0]);
		close(m->org_fd[1]);
		execute_command(mini->ast->pipeline->right, env, mini);
		cleanup(mini);
		exit(0);
	}
	else if (pid2 < 0)
	{
		perror("fork");
		return (-1);
	}
	return (pid2);
}

int	execute_pipeline(char **env, t_minishell *mini, t_cmd *m)
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
	pid1 = execute_left_command(m, pipefd, env, mini);
	pid2 = execute_right_command(m, pipefd, env, mini);
	if (pid1 > 0)
	{
		waitpid(pid1, &status, 0);
		if (WIFEXITED(status))
			mini->exit = WEXITSTATUS(status);
		close(pipefd[0]);
		close(pipefd[1]);
	}
	if (pid2 > 0)
	{
		waitpid(pid2, &status, 0);
		if (WIFEXITED(status))
			mini->exit = WEXITSTATUS(status);
		close(pipefd[0]);
		close(pipefd[1]);
	}
	return (0);
}
