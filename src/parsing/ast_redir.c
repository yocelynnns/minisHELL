/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:03:34 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/11 17:04:04 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	handle_redirect(t_token **tokens, t_ast_node *command_node)
{
	t_ast_node	*redirect_node;
	t_ast_node	*last_redirect;

	redirect_node = parse_redirect(tokens);
	if (!redirect_node)
		return (0);
	if (!command_node->command->redirect)
		command_node->command->redirect = redirect_node;
	else
	{
		last_redirect = command_node->command->redirect;
		while (last_redirect->redirect->next)
			last_redirect = last_redirect->redirect->next;
		last_redirect->redirect->next = redirect_node;
	}
	if (redirect_node->redirect->type == HEREDOC)
		command_node->command->heredoc = read_heredoc \
		(redirect_node->redirect->file);
	return (1);
}

static int	validate_redirect_type(t_token **tokens, t_ast_node *redirect_node)
{
	if ((*tokens)->type == REDIRECT_IN || (*tokens)->type == \
	REDIRECT_OUT || (*tokens)->type == APPEND || (*tokens)->type == HEREDOC)
	{
		redirect_node->redirect->type = (*tokens)->type;
		*tokens = (*tokens)->next;
		return (1);
	}
	fprintf(stderr, "Error: Unexpected token in \
	redirection: %s\n", (*tokens)->value);
	return (0);
}

static int	capture_redirect_file(t_token **tokens, t_ast_node *redirect_node)
{
	if (*tokens && (*tokens)->type == WORD)
	{
		redirect_node->redirect->file = strdup((*tokens)->value);
		*tokens = (*tokens)->next;
		return (1);
	}
	fprintf(stderr, "Error: Expected a filename after redirection.\n");
	return (0);
}
