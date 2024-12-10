/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 19:09:35 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/10 16:02:39 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_ast_node	*create_ast_node(t_ast_node_type type)
{
	t_ast_node	*new_node;

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
		if (!handle_redirect(tokens, command_node))
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

t_ast_node	*build_ast(t_token *tokens)
{
	return (parse_pipeline(&tokens));
}
