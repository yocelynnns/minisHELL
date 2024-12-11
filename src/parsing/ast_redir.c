/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:03:34 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/11 19:45:36 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	ast_redirect(t_token **tokens, t_ast_node *command_node)
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
