/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 19:09:35 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/13 00:50:13 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_ast_node *create_ast_node(t_ast_node_type type)
{
    t_ast_node *new_node;

    new_node = malloc(sizeof(t_ast_node));
    if (!new_node)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_node->type = type;
    new_node->word = NULL;
    new_node->pipeline = NULL;
    new_node->command = NULL;
    new_node->redirect = NULL;
    return (new_node);
}

// Parse a pipeline node
t_ast_node *parse_pipeline(t_token **tokens)
{
	t_ast_node *left = parse_command(tokens);
	t_ast_node *pipeline_node;

	if (!left)
		return NULL;

	while (*tokens && (*tokens)->type == PIPE)
	{
		*tokens = (*tokens)->next;
		t_ast_node *right = parse_command(tokens);
		if (!right)
		{
			free_ast(left);
			return NULL;
		}
		pipeline_node = create_ast_node(AST_PIPELINE);
		pipeline_node->pipeline = malloc(sizeof(t_ast_pipeline));
		pipeline_node->pipeline->left = left;
		pipeline_node->pipeline->right = right;
		left = pipeline_node;
	}
	return left;
}

// Parse a command node
t_ast_node *parse_command(t_token **tokens)
{
	t_ast_node *cmd = create_ast_node(AST_COMMAND);
	cmd->command = malloc(sizeof(t_ast_command));
	cmd->command->args = malloc(sizeof(char *) * 10);
	cmd->command->redirect = NULL;
	int arg_count = 0;

	while (*tokens && is_redirect((*tokens)->type))
	{
		if (!handle_redirect(cmd, tokens))
			return NULL;
	}

	while (*tokens && (*tokens)->type == WORD)
	{
		cmd->command->args[arg_count++] = strdup((*tokens)->value);
		*tokens = (*tokens)->next;
	}
	cmd->command->args[arg_count] = NULL;

	while (*tokens && is_redirect((*tokens)->type))
	{
		if (!handle_redirect(cmd, tokens))
			return NULL;
	}
	return cmd;
}

// Parse a redirect node
t_ast_node *parse_redirect(t_token **tokens)
{
	t_ast_node *redirect_node = create_ast_node(AST_REDIRECT);
	redirect_node->redirect = malloc(sizeof(t_ast_redirect));

	redirect_node->redirect->type = (*tokens)->type;
	*tokens = (*tokens)->next;

	if (*tokens && (*tokens)->type == WORD)
	{
		redirect_node->redirect->file = strdup((*tokens)->value);
		*tokens = (*tokens)->next;
	}
	else
	{
		fprintf(stderr, "Error: Expected filename after redirection.\n");
		free_ast(redirect_node);
		return NULL;
	}
	return redirect_node;
}

t_ast_node *build_ast(t_token *tokens)
{
    return (parse_pipeline(&tokens));
}
