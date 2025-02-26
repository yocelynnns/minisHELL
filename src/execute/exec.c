/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:08:26 by ysetiawa          #+#    #+#             */
/*   Updated: 2025/02/26 15:43:06 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	execute_command(t_ast_node *ast, t_minishell *mini)
{
	t_cmd	m;

	if (ast->type == AST_COMMAND)
	{
		m.org_fd[0] = dup(STDIN_FILENO);
		m.org_fd[1] = dup(STDOUT_FILENO);
		if (cmdchecks(ast, mini) < 0)
		{
			close(m.org_fd[0]);
			close(m.org_fd[1]);
			return ;
		}
		if (fork_and_execute(ast, mini, &m) < 0)
		{
			close(m.org_fd[0]);
			close(m.org_fd[1]);
			return ;
		}
		dup2(m.org_fd[0], STDIN_FILENO);
		dup2(m.org_fd[1], STDOUT_FILENO);
		close(m.org_fd[0]);
		close(m.org_fd[1]);
	}
	else if (ast->type == AST_PIPELINE)
		execute_pipeline(mini, ast);
}

int	check_dir_permission(char *args)
{
	struct stat	path_stat;

	if (stat(args, &path_stat) == -1)
	{
		write(2, args, ft_strlen(args));
		write(2, ": No such file or directory\n", 28);
		return (127);
	}
	if (S_ISDIR(path_stat.st_mode))
	{
		write(2, args, ft_strlen(args));
		write(2, ": Is a directory\n", 17);
		return (126);
	}
	if (access(args, X_OK) == -1)
	{
		perror(args);
		return (126);
	}
	return (0);
}

void	exec_cmd(t_ast_node *ast, t_minishell *mini, t_cmd *m, char *path)
{
	if (path && execve(path, ast->command->args, mini->env2) == -1)
	{
		perror("execve");
		fkoff(mini, m, EXIT_FAILURE);
	}
	if (errno == EACCES)
	{
		write(2, ast->command->args[0], ft_strlen(ast->command->args[0]));
		write(2, ": Permission denied\n", 20);
		fkoff(mini, m, 126);
	}
	write(2, ast->command->args[0], ft_strlen(ast->command->args[0]));
	write(2, ": command not found\n", 21);
	fkoff(mini, m, 127);
}

void	empty_cmd(t_ast_node *ast, t_minishell *mini, t_cmd *m)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (ast->command->args[i] != NULL)
	{
		if (ft_strlen(ast->command->args[i]) == 0)
		{
			free(ast->command->args[i]);
			ast->command->args[i] = NULL;
		}
		else
		{
			if (j != i)
				ast->command->args[j] = ast->command->args[i];
			j++;
		}
		i++;
	}
	ast->command->args[j] = NULL;
	if (j == 0 || !ast->command->args[0])
		fkoff(mini, m, EXIT_SUCCESS);
}

void	execute_in_child(t_ast_node *ast, t_minishell *mini, t_cmd *m)
{
	char	*executable_path;
	int		dir_status;

	empty_cmd(ast, mini, m);
	if (ast->command->args[0][0] == '/' || ast->command->args[0][0] == '.')
	{
		dir_status = check_dir_permission(ast->command->args[0]);
		if (dir_status == 126 || dir_status == 127)
			fkoff(mini, m, dir_status);
	}
	executable_path = get_executable_path(ast, mini);
	exec_cmd(ast, mini, m, executable_path);
}

// void	execute_in_child(t_ast_node *ast, t_minishell *mini, t_cmd *m)
// {
// 	char	*executable_path;
// 	int		dir_status;

// 	if (!ast->command->args[0] || ft_strlen(ast->command->args[0]) == 0)
// 		fkoff(mini, m, EXIT_SUCCESS);
// 	if ((ast->command->args[0][0] == '/' || ast->command->args[0][0] == '.')) 
// 	{
// 		dir_status = check_dir_permission(ast->command->args[0]);
// 		if (dir_status == 126)
// 			fkoff(mini, m, 126);
// 		if (dir_status == 127)
// 			fkoff(mini, m, 127);
// 	}
// 	executable_path = get_executable_path(ast, mini);
// 	if ((executable_path) && (execve(executable_path, ast->command->args, 
// 	mini->env2) == -1))
// 	{
// 		perror("execve");
// 		fkoff(mini, m, EXIT_FAILURE);
// 	}
// 	else
// 	{
// 		if (errno == EACCES)
// 		{
// 			write(2, ast->command->args[0], ft_strlen(ast->command->args[0]));
// 			write(2, ": Permission denied\n", 20);
// 			fkoff(mini, m, 126);
// 		}
// 		else
// 		{
// 			write(2, ast->command->args[0], ft_strlen(ast->command->args[0]));
// 			write(2, ": command not found\n", 21);
// 			fkoff(mini, m, 127);
// 		}
// 	}
// }

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