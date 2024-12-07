/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 19:09:35 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/08 00:36:15 by yocelynnns       ###   ########.fr       */
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

    // Parse the first command (with its redirections, if any)
    left_command = parse_command(tokens);
    if (!left_command)
        return (NULL);

    while (*tokens && (*tokens)->type == PIPE)
    {
        *tokens = (*tokens)->next;
        // Parse the command after the pipe
        right_command = parse_command(tokens);
        if (!right_command)
        {
            free_ast(left_command);
            return NULL;
        }

        // Create a pipeline node
        pipeline_node = create_ast_node(AST_PIPELINE);
        pipeline_node->pipeline = malloc(sizeof(t_ast_pipeline));
        pipeline_node->pipeline->left = left_command;
        pipeline_node->pipeline->right = right_command;

        // Update left_command to the new pipeline node
        left_command = pipeline_node;
    }
    return (left_command);
}

t_ast_node *parse_command(t_token **tokens)
{
    t_ast_node *command_node;
    t_ast_node *redirect_node;
    int arg_count = 0;

    if (!tokens || !*tokens)
        return (NULL);

    // Initialize the command node
    command_node = create_ast_node(AST_COMMAND);
    command_node->command = malloc(sizeof(t_ast_command));
    command_node->command->args = malloc(sizeof(char *) * 10); // allocate space for 10 args
    command_node->command->redirect = NULL; // init redirect to NULL
    arg_count = 0;

    // Parse leading redirection tokens (e.g., < infile, >> outfile)
    while (*tokens && ((*tokens)->type == REDIRECT_IN || (*tokens)->type == REDIRECT_OUT ||
                    (*tokens)->type == APPEND || (*tokens)->type == HEREDOC))
    {
        redirect_node = parse_redirect(tokens);
        if (!redirect_node)
        {
            free_ast(command_node);
            return (NULL);
        }

        // Attach redirect to command
        if (!command_node->command->redirect)
            command_node->command->redirect = redirect_node;
        else
        {
            // If there is already a redirection, chain the new one
            t_ast_node *last_redirect = command_node->command->redirect;
            while (last_redirect->redirect->next)
                last_redirect = last_redirect->redirect->next;
            last_redirect->redirect->next = redirect_node;
        }
        // If it's a HEREDOC, read the content
        if (redirect_node->redirect->type == HEREDOC) {
            command_node->command->heredoc = read_heredoc(redirect_node->redirect->file); // Implement this function
        }
    }

    // Parse command arguments
    while (*tokens && (*tokens)->type == WORD)
    {
        command_node->command->args[arg_count++] = strdup((*tokens)->value);
        *tokens = (*tokens)->next;
    }
    command_node->command->args[arg_count] = NULL;

    // Check for inline redirections
    while (*tokens && ((*tokens)->type == REDIRECT_IN || (*tokens)->type == REDIRECT_OUT ||
                   (*tokens)->type == APPEND || (*tokens)->type == HEREDOC))
    {
        redirect_node = parse_redirect(tokens);
        if (!redirect_node)
        {
            free_ast(command_node);
            return (NULL);
        }

        // Attach inline redirects
        if (!command_node->command->redirect)
            command_node->command->redirect = redirect_node;
        else
        {
            t_ast_node *last_redirect = command_node->command->redirect;
            while (last_redirect->redirect->next)
                last_redirect = last_redirect->redirect->next;
            last_redirect->redirect->next = redirect_node;
        }
        // If it's a HEREDOC, read the content
        if (redirect_node->redirect->type == HEREDOC) {
            command_node->command->heredoc = read_heredoc(redirect_node->redirect->file); // Implement this function
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

    // Check for valid redirect types
    if ((*tokens)->type == REDIRECT_IN || (*tokens)->type == REDIRECT_OUT ||
        (*tokens)->type == APPEND || (*tokens)->type == HEREDOC)
    {
        redirect_node->redirect->type = (*tokens)->type;
        *tokens = (*tokens)->next;

        // Validate and capture the filename
        if (*tokens && (*tokens)->type == WORD)
        {
            redirect_node->redirect->file = strdup((*tokens)->value);
            *tokens = (*tokens)->next;
        }
        else
        {
            fprintf(stderr, "Error: Expected a filename after redirection.\n");
            free_ast(redirect_node);
            return (NULL);
        }
    }
    else
    {
        fprintf(stderr, "Error: Unexpected token in redirection: %s\n", (*tokens)->value);
        free_ast(redirect_node);
        return (NULL);
    }

    return redirect_node;
}

t_ast_node *build_ast(t_token *tokens)
{
    return (parse_pipeline(&tokens));
}
