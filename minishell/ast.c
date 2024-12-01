/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 19:09:35 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/01 23:00:35 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void print_ast(t_ast_node *node, int depth)
{
    int i;
    int j;
    int k;
    
    if (!node) 
        return;

    i = 0; 
    while (i < depth)
    {
        printf("  ");
        i++;
    }

    if (node->type == AST_PIPELINE)
    {
        printf("PIPELINE\n");
        print_ast(node->pipeline->left, depth + 1);
        print_ast(node->pipeline->right, depth + 1);
    }
    else if (node->type == AST_COMMAND)
    {
        printf("COMMAND: [");
        j = 0;
        while (node->command->args[j])
        {
            printf("\"%s\"", node->command->args[j]);
            if (node->command->args[j + 1])
                printf(", ");
            j++;
        }
        printf("]\n");

        if (node->command->redirect)
            print_ast(node->command->redirect, depth + 1);
    }
    else if (node->type == AST_REDIRECT)
    {
        printf("REDIRECT:\n");
        k = 0;
        while (k < depth + 1)
        {
            printf("  ");
            k++;
        }
        if (node->redirect->type == REDIRECT_OUT)
            printf("TYPE: >\n");
        else
            printf("TYPE: <\n");
        printf("FILE: \"%s\"\n", node->redirect->file);
    }
    else if (node->type == AST_WORD)
        printf("WORD: \"%s\"\n", node->word);
    else
        printf("UNKNOWN NODE TYPE\n");
}

t_ast_node *parse_command_line(t_token **tokens)
{
    return (parse_pipeline(tokens));
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
    return (parse_command_line(&tokens));
}
