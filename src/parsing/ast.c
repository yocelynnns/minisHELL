/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 19:09:35 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/11 17:04:09 by ysetiawa         ###   ########.fr       */
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

t_ast_node	*build_ast(t_token *tokens)
{
	return (parse_pipeline(&tokens));
}
