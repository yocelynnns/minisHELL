/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:08:26 by ysetiawa          #+#    #+#             */
/*   Updated: 2025/02/24 19:28:23 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int is_directory(const char *path)
{
	struct stat statbuf;

	if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
		return (1);
	return (0);
}

void execute_command(t_ast_node *ast, t_minishell *mini)
{
	t_cmd m;

	if (ast->type == AST_COMMAND)
	{
		m.org_fd[0] = dup(STDIN_FILENO);
		m.org_fd[1] = dup(STDOUT_FILENO);
		if (cmdchecks(ast, mini) < 0)
		{
			close(m.org_fd[0]);
			close(m.org_fd[1]);
			return;
		}
		if (fork_and_execute(ast, mini, &m) < 0)
		{
			close(m.org_fd[0]);
			close(m.org_fd[1]);
			return;
		}
		dup2(m.org_fd[0], STDIN_FILENO);
		dup2(m.org_fd[1], STDOUT_FILENO);
		close(m.org_fd[0]);
		close(m.org_fd[1]);
	}
	else if (ast->type == AST_PIPELINE)
		execute_pipeline(mini, ast);
}

void pipe_exec_cmd(t_ast_node *ast, t_minishell *mini)
{
	t_cmd m;

	m.org_fd[0] = dup(STDIN_FILENO);
	m.org_fd[1] = dup(STDOUT_FILENO);
	if (cmdchecks(ast, mini) < 0)
		fkoff(mini, &m, EXIT_FAILURE);
	if (handle_builtin_commands(ast, mini, &m) == 0)
		fkoff(mini, &m, EXIT_SUCCESS);
	init_signals();
	close(m.org_fd[0]);
	close(m.org_fd[1]);
	execute_in_child(ast, mini, &m);
	dup2(m.org_fd[0], STDIN_FILENO);
	dup2(m.org_fd[1], STDOUT_FILENO);
	close(m.org_fd[0]);
	close(m.org_fd[1]);
}

void fkoff(t_minishell *mini, t_cmd *m, int returnval)
{
	mini->exit = returnval;
	close(m->org_fd[0]);
	close(m->org_fd[1]);
	cleanup(mini);
	exit(returnval);
}
int check_dir_permission(char *args)
{
	struct stat path_stat;

	if (stat(args, &path_stat) == -1)
		return (127);
	if (S_ISDIR(path_stat.st_mode))
	{
		write(2, args, ft_strlen(args));
		write(2, ": Is a directory\n", 17);
		return (126);
	}
	if (access(args, X_OK) == -1)
	{
		perror("access");
		if (errno == EACCES)
			return (126);
		return (127);
	}
	return (0);
}

void execute_in_child(t_ast_node *ast, t_minishell *mini, t_cmd *m)
{
	char *executable_path;

	if (!ast->command->args[0] || ft_strlen(ast->command->args[0]) == 0)
		fkoff(mini, m, EXIT_SUCCESS);
	if (is_directory(ast->command->args[0]))
	{
		int dir_status = check_dir_permission(ast->command->args[0]);
		if (dir_status)
			fkoff(mini, m, dir_status);
	}
	else 
		executable_path = get_executable_path(ast, mini);
	if ((executable_path) && (execve(executable_path, ast->command->args, mini->env2) == -1))
	{
		perror("execve");
		fkoff(mini, m, EXIT_FAILURE);
	}
	else
	{
		write(2, "Command not found: ", 19);
		write(2, ast->command->args[0], ft_strlen(ast->command->args[0]));
		write(2, "\n", 1);
		fkoff(mini, m, 127);
	}
}

// void execute_in_child(t_ast_node *ast, t_minishell *mini, t_cmd *m)
// {
// 	char *args[number_of_args(ast->command->args)];

// 	if (!ast->command->args[0] || ft_strlen(ast->command->args[0]) == 0)
// 		fkoff(mini, m, EXIT_SUCCESS);

// 	args[0] = ft_strdup(ast->command->args[0]);

// 	if (args[0][0] != '.' && args[0][0] != '/' &&
// 		ft_strcmp(ft_substr(ast->command->args[0], 0, 5), "/bin/") != 0)
// 		args[0] = ft_strjoin("/bin/", ast->command->args[0]);

// 	int dir_status = check_directory(args[0]);
// 	if (dir_status == 126)
// 		fkoff(mini, m, 126);

// 	int perm_status = check_directory(args[0]);
// 	if (perm_status)
// 		fkoff(mini, m, perm_status);

// 	mini->env2 = env_list_to_array(mini->env);

// 	if (execve(args[0], ast->command->args, mini->env2) == -1)
// 	{
// 		perror("execve");
// 		fkoff(mini, m, 127);
// 	}

// 	write(2, "Command not found: ", 19);
// 	write(2, ast->command->args[0], ft_strlen(ast->command->args[0]));
// 	write(2, "\n", 1);
// 	fkoff(mini, m, 127);
// }

// void	expand_variables_in_args(char **args, t_env *env)
// {
// 	int		i;
// 	char	*expanded_arg;

// 	i = 0;
// 	while (args[i])
// 	{
// 		expanded_arg = expand_argument(args[i], env);
// 		if (expanded_arg)
// 		{
// 			free(args[i]);
// 			args[i] = expanded_arg;
// 		}
// 		i++;
// 	}
// }

// if (ast->command->redirect)
// 	handle_all_redirections(ast, mini);
// if (ft_strcmp(ast->command->args[0], "cd") == 0)
// 	ft_cd(ast->command->args, mini->env);
// else if (ft_strcmp(ast->command->args[0], "export") == 0)
// {
// 	if (!ast->command->redirect)
// 		ft_export(ast->command->args, &mini->env);
// }
// else if (ft_strcmp(ast->command->args[0], "unset") == 0)
// {
// 	if (!ast->command->redirect)
// 		ft_unset(ast->command->args, mini);
// }
// else if (ft_strcmp(ast->command->args[0], "exit") == 0)
// 	ft_exit(ast->command->args, mini);

// printf("Command not found: %s\n", ast->command->args[0]);
// expand_variables_in_args(ast->command->args, mini.env);