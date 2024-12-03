/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:51:00 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/03 15:51:03 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int main()
{
    char *input;
    t_token *tokens;
    t_ast_node *ast;
    
    // CAN DELTE THIS IF YOU WANT YOCE

    printf("Test 1: Simple echo\n");
    char *args1[] = {"echo", "Hello", "world", NULL};
    ft_echo(args1);  // Should print "Hello world"

    printf("\nTest 2: Echo with -n option\n");
    char *args2[] = {"echo", "-n", "Hello", "world", NULL};
    ft_echo(args2);  // Should print "Hello world" without a newline

    printf("\nTest 3: Echo with only -n\n");
    char *args3[] = {"echo", "-n", NULL};
    ft_echo(args3);

    printf("Testing pwd:\n");
    ft_pwd();
    
    // CAN DELTE THIS IF YOU WANT YOCE
    
    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (strcmp(input, "exit") == 0)
        {
            free(input);
            printf("exit\n");
            break;
        }
        else if (strcmp(input, "clear") == 0)
            printf("\033[H\033[J");
            
        if (*input)
            add_history(input);

        tokens = lexer(input);
        if (!tokens)
        {
            free(input);
            continue;
        }
        ast = build_ast(tokens);

        // test
        print_tokens(tokens);
        print_ast(ast, 0);

        free_tokens(tokens);
        free_ast(ast);
        free(input);
    }
    return (0);
}
