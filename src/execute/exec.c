/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:08:26 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/11 17:00:28 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
