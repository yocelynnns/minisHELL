/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 00:48:44 by yocelynnns        #+#    #+#             */
/*   Updated: 2024/12/14 18:50:24 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Check if a token is a redirect
int	is_redirect(int type)
{
	return (type == REDIRECT_IN || type == REDIRECT_OUT || \
	type == APPEND || type == HEREDOC);
}

// Attach a redirect to a command
void	attach_redirect(t_ast_node *cmd, t_ast_node *redirect_node)
{
	t_ast_node	*last;

	if (!cmd->command->redirect)
		cmd->command->redirect = redirect_node;
	else
	{
		last = cmd->command->redirect;
		while (last->redirect->next)
			last = last->redirect->next;
		last->redirect->next = redirect_node;
	}
}

// Handle a redirect during command parsing
int	handle_redirect(t_ast_node *cmd, t_token **tokens)
{
	t_ast_node	*redirect_node;

	redirect_node = parse_redirect(tokens);
	if (!redirect_node)
	{
		free_ast(cmd);
		return (0);
	}
	attach_redirect(cmd, redirect_node);
	if (redirect_node->redirect->type == HEREDOC)
		cmd->command->heredoc = read_heredoc(redirect_node->redirect->file);
	return (1);
}
