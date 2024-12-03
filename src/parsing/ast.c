/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 19:09:35 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/03 15:47:37 by ysetiawa         ###   ########.fr       */
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

t_ast_node *parse_pipeline(t_token **tokens)
{
    t_ast_node *left_command;
    t_ast_node *right_command;
    t_ast_node *pipeline_node;

    // parse first command
    left_command = parse_command(tokens);
    if (!left_command)
        return (NULL);

    while (*tokens && (*tokens)->type == PIPE)
    {
        *tokens = (*tokens)->next;
        // parse next cmd aft pipe
        right_command = parse_command(tokens);
        if (!right_command)
        {
            free_ast(left_command);
            return NULL;
        }

        pipeline_node = create_ast_node(AST_PIPELINE);
        pipeline_node->pipeline = malloc(sizeof(t_ast_pipeline));
        pipeline_node->pipeline->left = left_command;
        pipeline_node->pipeline->right = right_command;

        // The new pipeline node becomes the left command for the next pipe
        left_command = pipeline_node;
    }
    return (left_command);
}

t_ast_node *parse_command(t_token **tokens)
{
    t_ast_node *command_node;
    int arg_count = 0;
    
    if (!tokens || !*tokens)
        return (NULL);

    command_node = create_ast_node(AST_COMMAND);
    command_node->command = malloc(sizeof(t_ast_command));
    command_node->command->args = malloc(sizeof(char *) * 10);  // ex: alloc space for 10 args
    command_node->command->redirect = NULL;  // init redirect to NULL
    arg_count = 0;

    // parse the cmd args
    while (*tokens && (*tokens)->type != PIPE && (*tokens)->type != REDIRECT_IN && (*tokens)->type != REDIRECT_OUT)
    {
        if ((*tokens)->type == WORD)
            command_node->command->args[arg_count++] = strdup((*tokens)->value);
        *tokens = (*tokens)->next;
    }
    command_node->command->args[arg_count] = NULL; 

    // check for redirection
    if (*tokens && ((*tokens)->type == REDIRECT_IN || (*tokens)->type == REDIRECT_OUT))
    {
        command_node->command->redirect = parse_redirect(tokens);
        if (!command_node->command->redirect)
        {
            free_ast(command_node);
            return (NULL);
        }
    }
    return (command_node);
}

t_ast_node *parse_redirect(t_token **tokens)
{
    t_ast_node *redirect_node;
    
    if (!tokens || !*tokens)
        return (NULL);

    redirect_node = create_ast_node(AST_REDIRECT);
    redirect_node->redirect = malloc(sizeof(t_ast_redirect));

    // check for redirect token
    if ((*tokens)->type != REDIRECT_IN && (*tokens)->type != REDIRECT_OUT)
    {
        fprintf(stderr, "Error: Expected a redirect token.\n");
        free_ast(redirect_node);
        return NULL;
    }

    // move past the redirect token
    redirect_node->redirect->type = (*tokens)->type;
    *tokens = (*tokens)->next;

    // check the filename
    if (*tokens && (*tokens)->type == WORD)
    {
        redirect_node->redirect->file = strdup((*tokens)->value);
        *tokens = (*tokens)->next;
    }
    else
    {
        if (redirect_node->redirect->type == REDIRECT_OUT)
            fprintf(stderr, "Error: Expected a file name after '>'.\n");
        else
            fprintf(stderr, "Error: Expected a file name after '<'.\n");
        free_ast(redirect_node);
        return NULL;
    }

    return (redirect_node);
}

t_ast_node *build_ast(t_token *tokens)
{
    return (parse_pipeline(&tokens));
}
