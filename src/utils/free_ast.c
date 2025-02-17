/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 00:25:55 by yocelynnns        #+#    #+#             */
/*   Updated: 2025/02/18 00:53:56 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	free_command(t_ast_node *node)
{
	int	i;

	if (!node->command)
		return ;
	if (node->command->args)
	{
		i = 0;
		while (node->command->args[i])
		{
			free(node->command->args[i]);
			node->command->args[i] = NULL;
			i++;
		}
		free(node->command->args);
		node->command->args = NULL;
	}
	free_ast(node->command->redirect);
	if (node->command->heredoc)
		free(node->command->heredoc);
	node->command->heredoc = NULL;
	free(node->command);
	node->command = NULL;
}

static void	free_pipeline(t_ast_node *node)
{
	if (!node->pipeline)
		return ;
	free_ast(node->pipeline->left);
	free_ast(node->pipeline->right);
	free(node->pipeline);
}

static void	free_redirect(t_ast_node *node)
{
	if (!node->redirect)
		return ;
	free(node->redirect->file);
	node->redirect->file = NULL;
	free_ast(node->redirect->next);
	free(node->redirect);
	node->redirect = NULL;
}

void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == AST_COMMAND)
		free_command(node);
	else if (node->type == AST_PIPELINE)
		free_pipeline(node);
	else if (node->type == AST_REDIRECT)
		free_redirect(node);
	else if (node->type == AST_WORD)
		free(node->word);
	free(node);
}
