/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:22:59 by yocelynnns        #+#    #+#             */
/*   Updated: 2025/02/20 20:06:29 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*concat_path(char *dir, char *cmd)
{
	size_t	dir_len;
	size_t	cmd_len;
	size_t	total_len;
	char	*full_path;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	total_len = dir_len + cmd_len + 2;
	full_path = malloc(total_len);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	return (full_path);
}

char	*check_directory(char *dir, char *cmd, t_minishell *mini) // We need to use this function to check the permission of the direcory
{
	char	*full_path;
	int		i;

	full_path = concat_path(dir, cmd);
	if (!full_path)
		return (NULL);
	i = access(full_path, X_OK);
	if (i != 0)
	{
		// ft_putstr_fd("path: no such file or directory: ", STDERR);
		mini->exit = 126;
	}
	if (i == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*find_executable(char *cmd, t_minishell *mini)
{
	char	*path_env;
	char	**dirs;
	char	*full_path;
	int		i;

	path_env = get_env_value("PATH", mini->env);
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	full_path = NULL;
	i = 0;
	while (dirs[i] != NULL)
	{
		full_path = check_directory(dirs[i], cmd, mini);
		if (full_path)
		{
			free_dirs(dirs);
			return (full_path);
		}
		i++;
	}
	free_dirs(dirs);
	return (NULL);
}

char	*get_executable_path(t_ast_node *ast, t_minishell *mini)
{
	char	*command;
	char	*executable_path;

	command = ast->command->args[0];
	if (command && (command[0] == '/' || command[0] == '.'))
	{
		if (access(command, X_OK) == 0)
			return (command);
		else
		{
			ft_putstr_fd(": no such file or directory. ", STDERR);
			mini->exit = 126;
		}
	}
	else
	{
		executable_path = find_executable(command, mini);
		if (executable_path)
			return (executable_path);
	}
	return (NULL);
}

void	handle_fork_signals(t_minishell *mini, t_cmd *m)
{
	int	signal;

	signal = WTERMSIG(m->status);
	if (signal == SIGINT)
	{
		g_sigint = 1;
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
	}
	else if (signal == SIGQUIT)
	{
		mini->exit = 131;
		if (WCOREDUMP(m->status))
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
		else
			write(STDERR_FILENO, "Quit\n", 5);
	}
}

// int	fork_and_execute(t_ast_node *ast, t_minishell *mini, t_cmd *m)
// {
// 	pid_t	pid;
// 	int		signal;

// 	if (handle_builtin_commands(ast, mini, m) == 0)
// 		return (mini->exit);
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		init_signals();
// 		close(m->org_fd[0]);
// 		close(m->org_fd[1]);
// 		execute_in_child(ast, mini, m);
// 	}
// 	else if (pid < 0)
// 	{
// 		perror("fork");
// 		return (-1);
// 	}
// 	waitpid(pid, &m->status, 0);
// 	if (WIFSIGNALED(m->status))
// 	{
// 		signal = WTERMSIG(m->status);
// 		if (signal == g_sigint)
// 		{
// 			g_sigint = 1;
// 			mini->exit = 130;
// 			ft_putstr_fd("\n", STDOUT_FILENO);
// 			rl_on_new_line();
// 		}
// 		else if (signal == SIGQUIT)
// 		{
// 			mini->exit = 131;
// 			if (WCOREDUMP(m->status))
// 				write(STDERR_FILENO, "Quit (core dumped)\n", 20);
// 			else
// 				write(STDERR_FILENO, "Quit\n", 5);
// 			return (131);
// 		}
// 	}
// 	else if (WIFEXITED(m->status))
// 		mini->exit = WEXITSTATUS(m->status);
// 	return (mini->exit);
// }
