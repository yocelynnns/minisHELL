/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:01:21 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/11 19:47:21 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
		redirect_node->redirect->file = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
		return (1);
	}
	fprintf(stderr, "Error: Expected a filename after redirection.\n");
	return (0);
}

t_ast_node	*parse_redirect(t_token **tokens)
{
	t_ast_node	*redirect_node;

	if (!tokens || !*tokens)
		return (NULL);
	redirect_node = create_ast_node(AST_REDIRECT);
	redirect_node->redirect = malloc(sizeof(t_ast_redirect));
	if (!validate_redirect_type(tokens, redirect_node) \
	|| !capture_redirect_file(tokens, redirect_node))
	{
		free_ast(redirect_node);
		return (NULL);
	}
	return (redirect_node);
}