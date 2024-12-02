/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:41:11 by messs             #+#    #+#             */
/*   Updated: 2024/12/03 15:50:48 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void free_env(t_env *env) {
    t_env *tmp;

    while (env) {
        tmp = env->next;    // Save the next node
        free(env->value);   // Free the key-value string
        free(env);          // Free the current node
        env = tmp;          // Move to the next node
    }
}

void free_tokens(t_token *tokens)
{
    t_token *temp;
    
    while (tokens)
    {
        temp = tokens;
        tokens = tokens->next;
        free(temp->value);
        free(temp);
    }
}

void free_ast(t_ast_node *node)
{
    int i;
    
    if (!node)
        return;

    if (node->type == AST_COMMAND)
    {
        if (node->command)
        {
            if (node->command->args)
            {
                i = 0;
                while (node->command->args[i])
                {
                    free(node->command->args[i]);
                    i++;
                }
                free(node->command->args);
            }
            free(node->command->redirect);
            free(node->command);
        }
    }

    if (node->type == AST_PIPELINE)
    {
        if (node->pipeline)
        {
            free_ast(node->pipeline->left);
            free_ast(node->pipeline->right);
            free(node->pipeline);
        }
    }

    if (node->type == AST_REDIRECT)
    {
        if (node->redirect)
        {
            free(node->redirect->file);
            free(node->redirect);
        }
    }

    if (node->type == AST_WORD)
        free(node->word);

    free(node);
}