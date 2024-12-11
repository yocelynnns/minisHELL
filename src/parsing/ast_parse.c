/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:01:21 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/11 17:15:38 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_ast_node	*parse_pipeline(t_token **tokens)
{
	t_ast_node	*left_command;
	t_ast_node	*right_command;
	t_ast_node	*pipeline_node;

	left_command = parse_command(tokens);
	if (!left_command)
		return (NULL);
	while (*tokens && (*tokens)->type == PIPE)
	{
		*tokens = (*tokens)->next;
		right_command = parse_command(tokens);
		if (!right_command)
		{
			free_ast(left_command);
			return (NULL);
		}
		pipeline_node = create_ast_node(AST_PIPELINE);
		pipeline_node->pipeline = malloc(sizeof(t_ast_pipeline));
		pipeline_node->pipeline->left = left_command;
		pipeline_node->pipeline->right = right_command;
		left_command = pipeline_node;
	}
	return (left_command);
}

static void	parse_arguments(t_token **tokens, t_ast_node *command_node)
{
	int	arg_count;

	arg_count = 0;
	while (*tokens && (*tokens)->type == WORD)
	{
		command_node->command->args[arg_count++] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
	}
	command_node->command->args[arg_count] = NULL;
}

static int	parse_all_redirects(t_token **tokens, t_ast_node *command_node)
{
	while (*tokens && ((*tokens)->type == REDIRECT_IN || (*tokens)->type \
	== REDIRECT_OUT || (*tokens)->type == APPEND || (*tokens)->type == HEREDOC))
	{
		if (!ast_redirect(tokens, command_node))
		{
			free_ast(command_node);
			return (0);
		}
	}
	return (1);
}

t_ast_node	*parse_command(t_token **tokens)
{
	t_ast_node	*command_node;

	if (!tokens || !*tokens)
		return (NULL);
	command_node = create_ast_node(AST_COMMAND);
	command_node->command = malloc(sizeof(t_ast_command));
	command_node->command->args = malloc(sizeof(char *) * 10);
	command_node->command->redirect = NULL;
	if (!parse_all_redirects(tokens, command_node))
		return (NULL);
	parse_arguments(tokens, command_node);
	if (!parse_all_redirects(tokens, command_node))
		return (NULL);
	return (command_node);
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
