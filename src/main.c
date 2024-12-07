/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:51:00 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/07 18:58:21 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int main(int ac, char **av, char **env)
{
    char *input;
    t_token *tokens;
    t_ast_node *ast;
    t_minishell mini;
    int exit_status;

    (void)ac;
    (void)av;

    // Initialize environment variables
    if (!env_init(&mini, env))
    {
        fprintf(stderr, "Error: Failed to initialize environment variables\n");
        return EXIT_FAILURE;
    }

    while (1)
    {
        // Read input from the user
        input = readline("minishell$ ");
        if (!input)
        {
            printf("exit\n");
            break;
        }

        // Add input to history if it's not empty
        if (*input)
            add_history(input);

        // Tokenize the input
        tokens = lexer(input);
        if (!tokens)
        {
            free(input);
            continue;
        }

        // Build the abstract syntax tree (AST)
        ast = build_ast(tokens);
        if (!ast)
        {
            free_tokens(tokens);
            free(input);
            continue;
        }

        // test
        // print_tokens(tokens);
        // print_ast(ast, 0);

        // Execute the command(s) represented by the AST
        exit_status = execute_command(ast, env);
        if (exit_status == 1)
        {
            free_tokens(tokens);
            free_ast(ast);
            free(input);
            printf("exit\n");
            break;
        }

        // Free allocated resources
        free_tokens(tokens);
        free_ast(ast);
        free(input);
    }

    // Free the environment
    free_env(mini.env);

    return exit_status;
}

    // CAN DELTE THIS IF YOU WANT YOCE

    // printf("Test 1: Simple echo\n");
    // char *args1[] = {"echo", "Hello", "world", NULL};
    // ft_echo(args1);  // Should print "Hello world"

    // printf("\nTest 2: Echo with -n option\n");
    // char *args2[] = {"echo", "-n", "Hello", "world", NULL};
    // ft_echo(args2);  // Should print "Hello world" without a newline

    // printf("\nTest 3: Echo with only -n\n");
    // char *args3[] = {"echo", "-n", NULL};
    // ft_echo(args3);

    // printf("Testing pwd:\n");
    // ft_pwd();
    
    // CAN DELTE THIS IF YOU WANT YOCE

     // test
        // print_tokens(tokens);
        // print_ast(ast, 0);