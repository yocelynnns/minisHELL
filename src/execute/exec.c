/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:08:26 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/10 21:08:35 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*resize_buffer(char *buffer, size_t *current_size, \
size_t total_length)
{
	char	*new_buffer;

	*current_size *= 2;
	new_buffer = malloc(*current_size);
	if (!new_buffer)
	{
		perror("malloc");
		free(buffer);
		return (NULL);
	}
	ft_memcpy(new_buffer, buffer, total_length + 1);
	free(buffer);
	return (new_buffer);
}

static int	is_delimiter(const char *line, const char *delimiter, \
size_t delimiter_length)
{
	return (ft_strncmp(line, delimiter, delimiter_length) == 0 \
	&& (line[delimiter_length] == '\n' || line[delimiter_length] == '\0'));
}

char	*read_heredoc(const char *delimiter)
{
	char	*content;
	size_t	total_length;
	size_t	delimiter_length;
	size_t	current_size;
	ssize_t	bytes_read;
	char	*final_content;

	content = malloc(INITIAL_SIZE);
	total_length = 0;
	delimiter_length = ft_strlen(delimiter);
	current_size = INITIAL_SIZE;
	if (!content)
		return (perror("malloc"), NULL);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		bytes_read = read(STDIN_FILENO, content + total_length, \
		current_size - total_length - 1);
		if (bytes_read < 0)
			return (perror("read"), free(content), NULL);
		content[total_length + bytes_read] = '\0';
		if (is_delimiter(content + total_length, delimiter, delimiter_length))
			break ;
		total_length += bytes_read;
		if (total_length + 1 >= current_size)
		{
			content = resize_buffer(content, &current_size, total_length);
			if (!content)
				return (NULL);
		}
	}
	final_content = malloc(total_length + 1);
	if (!final_content)
		return (perror("malloc"), free(content), NULL);
	ft_memcpy(final_content, content, total_length + 1);
	free(content);
	return (final_content);
}

char	*concat_path(const char *dir, const char *cmd)
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
	strcpy(full_path, dir);
	strcat(full_path, "/");
	strcat(full_path, cmd);
	return (full_path);
}

static void	free_directories(char **dirs)
{
	int	i;

	i = 0;
	while (dirs[i] != NULL)
	{
		free(dirs[i]);
		i++;
	}
	free(dirs);
}

static char	*check_executable_in_dir(const char *dir, const char *cmd)
{
	char	*full_path;

	full_path = concat_path(dir, cmd);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*find_executable(const char *cmd)
{
	char	*path_env;
	char	**dirs;
	char	*full_path;
	int		i;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i] != NULL)
	{
		full_path = check_executable_in_dir(dirs[i], cmd);
		if (full_path)
		{
			free_directories(dirs);
			return (full_path);
		}
		i++;
	}
	free_directories(dirs);
	return (NULL);
}

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

static void	exec_command_or_path(t_ast_node *ast, char **env)
{
	char	*command;
	char	*executable_path;

	command = ast->command->args[0];
	if (command[0] == '/' && access(command, X_OK) == 0)
		execve(command, ast->command->args, env);
	executable_path = find_executable(command);
	if (!executable_path)
	{
		fprintf(stderr, "Command not found: %s\n", command);
		exit(EXIT_FAILURE);
	}
	execve(executable_path, ast->command->args, env);
	perror("execve");
	free(executable_path);
	exit(EXIT_FAILURE);
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

	if (ast->type == AST_COMMAND)
	{
        if (ft_strcmp(ast->command->args[0], "echo") == 0)
            return ft_echo(ast->command->args);
		else if (ft_strcmp(ast->command->args[0], "pwd") == 0)
            return ft_pwd();
		else if (ft_strcmp(ast->command->args[0], "exit") == 0)
            return (ft_exit(&mini, ast->command->args), 0);
		// else if (ft_strcmp(ast->command->args[0], "cd") == 0)
        //     return ft_cd(ast->command->args, &env_list);
		// else if (ft_strcmp(ast->command->args[0], "env") == 0)
        //     return ft_env(&env_list);
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
