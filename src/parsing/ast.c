/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 19:09:35 by ysetiawa          #+#    #+#             */
/*   Updated: 2025/02/17 23:50:47 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	parse_redirects(t_ast_node *cmd, t_token **tokens, \
	t_minishell *mini)
{
	while (*tokens && ((*tokens)->type == REDIRECT_IN || (*tokens)->type \
	== REDIRECT_OUT || (*tokens)->type == APPEND || (*tokens)->type == HEREDOC))
	{
		if (!handle_redirect(cmd, tokens, mini))
			return (0);
	}
	return (1);
}

static int	parse_args(t_ast_node *cmd, t_token **tokens, int *arg_count)
{
	while (*tokens && (*tokens)->type == WORD)
	{
		cmd->command->args[(*arg_count)++] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
	}
	return (1);
}

t_ast_node	*parse_command(t_token **tokens, t_minishell *mini, int i)
{
	t_ast_node	*cmd;
	int			arg_count;

	cmd = create_ast_node(AST_COMMAND);
	init_cmd(cmd, i);
	arg_count = 0;
	if (!parse_redirects(cmd, tokens, mini) || !parse_args(cmd, tokens, \
	&arg_count))
		return (NULL);
	if (!parse_redirects(cmd, tokens, mini) || !parse_args(cmd, tokens, \
	&arg_count))
		return (NULL);
	cmd->command->args[arg_count] = NULL;
	return (cmd);
}

t_ast_node	*parse_redirect(t_token **tokens, t_minishell *mini)
{
	t_ast_node	*redirect_node;

	redirect_node = create_ast_node(AST_REDIRECT);
	redirect_node->redirect = malloc(sizeof(t_ast_redirect));
	ft_memset(redirect_node->redirect, 0, sizeof(t_ast_redirect));
	redirect_node->redirect->next = NULL;
	redirect_node->redirect->type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == WORD)
	{
		redirect_node->redirect->file = ft_strdup((*tokens)->value);
		if (!redirect_node->redirect->file)
			return (free_ast(redirect_node), NULL);
		*tokens = (*tokens)->next;
	}
	else
	{
		printf("Error: Syntax error near unexpected token `newline'\n");
		free_ast(redirect_node);
		mini->exit = 2;
		return (NULL);
	}
	return (redirect_node);
}

// t_ast_node *parse_pipeline(t_token *tokens, t_minishell *mini, int i)
// {
//     t_ast_node *left;
//     t_ast_node *right;
//     t_ast_node *pipeline_node;

// 	if (!handle_pipe_error(tokens, mini))
// 	{
// 		// free_ast(left);
// 		return (NULL);
// 	}
//     left = parse_command(&tokens, mini, i);
//     if (!left)
//         return (NULL);
//     // while (*tokens && (*tokens)->type == PIPE)
//     // {
//         // *tokens = (*tokens)->next;
//         // right = parse_command(tokens, mini, i);
//         // if (!right)
//         // {
//         //     free_ast(left);
//         //     return (NULL);
//         // }
//         // pipeline_node = create_pipeline_node(left, right);
// 		if ((tokens) && tokens->type == PIPE)
//         	right = parse_pipeline(tokens->next, mini, i);
// 		else
// 			return (left);
//         if (!right)
//         {
//             free_ast(left);
//             return (NULL);
//         }
//         pipeline_node = create_pipeline_node(left, right);

//         left = pipeline_node;
//     // }
//     return (left);
// }

// t_ast_node	*parse_pipeline(t_token **tokens, t_minishell *mini, int i)
// {
// 	t_ast_node	*left;
// 	t_ast_node	*right;
// 	t_ast_node	*pipeline_node;

// 	left = parse_command(tokens, mini, i);
// 	if (!left)
// 		return (NULL);
// 	while (*tokens && (*tokens)->type == PIPE)
// 	{
// 		if ((*tokens)->next == NULL)
//         {
//             printf("Error: Syntax error near unexpected token '|'\n");
//             mini->exit = 2;
//             return (NULL);
//         }
// 		*tokens = (*tokens)->next;
// 		right = parse_command(tokens, mini, i);
// 		if (!right)
// 		{
// 			free_ast(left);
// 			return (NULL);
// 		}
// 		pipeline_node = create_ast_node(AST_PIPELINE);
// 		pipeline_node->pipeline = malloc(sizeof(t_ast_pipeline));
// 		pipeline_node->pipeline->left = left;
// 		pipeline_node->pipeline->right = right;
// 		left = pipeline_node;
// 	}
// 	return (left);
// }

// t_ast_node	*parse_command(t_token **tokens, t_minishell *mini, int i)
// {
// 	t_ast_node	*cmd;
// 	int			arg_count;

// 	cmd = create_ast_node(AST_COMMAND);
// 	init_cmd(cmd, i);
// 	arg_count = 0;
// 	while (*tokens && is_redirect((*tokens)->type))
// 	{
// 		if (!handle_redirect(cmd, tokens, mini))
// 			return (NULL);
// 	}
// 	while (*tokens && (*tokens)->type == WORD)
// 	{
// 		cmd->command->args[arg_count++] = ft_strdup((*tokens)->value);
// 		*tokens = (*tokens)->next;
// 	}
// 	while (*tokens && is_redirect((*tokens)->type))
// 	{
// 		if (!handle_redirect(cmd, tokens, mini))
// 			return (NULL);
// 	}
// 	while (*tokens && (*tokens)->type == WORD)
// 	{
// 		cmd->command->args[arg_count++] = ft_strdup((*tokens)->value);
// 		*tokens = (*tokens)->next;
// 	}
// 	cmd->command->args[arg_count] = NULL;
// 	return (cmd);
// }
