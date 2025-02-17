/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 00:48:44 by yocelynnns        #+#    #+#             */
/*   Updated: 2025/02/17 23:47:04 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	attach_redirect(t_ast_node *cmd, t_ast_node *redirect_node)
{
	t_ast_node	*last;

	if (!cmd->command->redirect)
		cmd->command->redirect = redirect_node;
	else
	{
		last = cmd->command->redirect;
		while (last->redirect && last->redirect->next)
			last = last->redirect->next;
		last->redirect->next = redirect_node;
	}
}

int	handle_redirect(t_ast_node *cmd, t_token **tokens, t_minishell *mini)
{
	t_ast_node	*redirect_node;

	redirect_node = parse_redirect(tokens, mini);
	if (!redirect_node)
	{
		free_ast(cmd);
		return (0);
	}
	attach_redirect(cmd, redirect_node);
	if (redirect_node->redirect->type == HEREDOC)
		cmd->command->heredoc = read_heredoc(redirect_node->redirect->file,
				mini);
	return (1);
}

t_ast_node	*create_ast_node(t_ast_node_type type)
{
	t_ast_node	*new_node;

	new_node = malloc(sizeof(t_ast_node));
	if (!new_node)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	ft_memset(new_node, 0, sizeof(t_ast_node));
	new_node->command = NULL;
	new_node->pipeline = NULL;
	new_node->redirect = NULL;
	new_node->word = NULL;
	new_node->type = type;
	return (new_node);
}

void	init_cmd(t_ast_node *cmd, int i)
{
	cmd->command = malloc(sizeof(t_ast_command));
	if (!cmd->command)
		return ;
	ft_memset(cmd->command, 0, sizeof(t_ast_command));
	cmd->command->heredoc = NULL;
	cmd->command->redirect = NULL;
	cmd->command->args = NULL;
	cmd->command->args = malloc(sizeof(char *) * (i + 1));
	if (!cmd->command->args)
	{
		free(cmd->command);
		cmd->command = NULL;
		return ;
	}
	ft_memset(cmd->command->args, 0, sizeof(char *) * (i + 1));
}

// int	is_redirect(int type)
// {
// 	return (type == REDIRECT_IN || type == REDIRECT_OUT || type == APPEND
// 		|| type == HEREDOC);
// }

// int	handle_redirect(t_ast_node *cmd, t_token **tokens, t_minishell *mini)
// {
// 	t_ast_node	*redirect_node;
// 	pid_t		pid;
// 	int			status;

// 	redirect_node = parse_redirect(tokens, mini);
// 	if (!redirect_node)
// 	{
// 		free_ast(cmd);
// 		return (0);
// 	}
// 	attach_redirect(cmd, redirect_node);
// 	if (redirect_node->redirect->type == HEREDOC)
// 	{
// 		pid = fork();
// 		if (pid == 0)
// 		{
// 			cmd->command->heredoc = read_heredoc(redirect_node->redirect->file,
// 					mini);
// 			if (!cmd->command->heredoc)
// 				exit(mini->exit);
// 			exit(0);
// 		}
// 		else if (pid < 0)
// 		{
// 			perror("fork");
// 			free_ast(cmd);
// 			return (0);
// 		}
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status))
// 		{
// 			mini->exit = WEXITSTATUS(status);
// 			if (mini->exit != 0)
// 			{
// 				free_ast(cmd);
// 				return (0);
// 			}
// 		}
// 		else if (WIFSIGNALED(status))
// 		{
// 			mini->exit = mini->exit;
// 			free_ast(cmd);
// 			return (0);
// 		}
// 	}
// 	return (1);
// }
