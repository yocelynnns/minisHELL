/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messs <messs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:51:00 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/02 12:37:55 by messs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// create a new token
t_token *create_token(t_token_type type, const char *value)
{
    t_token *new_token;
    
    new_token = malloc(sizeof(t_token));
    if (!new_token)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_token->type = type;
    new_token->value = strdup(value);
    new_token->next = NULL;
    return (new_token);
}

// add a token to the end of the token list
void add_token(t_token **head, t_token *new_token)
{
    t_token *temp;
    
    if (!*head)
        *head = new_token;
    else
    {
        temp = *head;
        while (temp->next)
            temp = temp->next;
        temp->next = new_token;
    }
}

// tokenize input and assign token types
t_token *lexer(const char *input)
{
    t_token *token_list = NULL;
    int i = 0;
    int start = 0;
    char quote = 0; // to track quotes
    int last_token_was_pipe = 0; // to track double pipes

    while (input[i])
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            if (quote == 0)
                quote = input[i]; // first quote
            else if (quote == input[i])
                quote = 0; // close quote
        }
        else if (isspace(input[i]) && !quote)
        {
            if (i > start)
                add_token(&token_list, create_token(WORD, strndup(input + start, i - start)));
            start = i + 1; // move to the next token
        }
        else if (input[i] == '<' && !quote)
        {
            if (i > start) // add the previous token
                add_token(&token_list, create_token(WORD, strndup(input + start, i - start)));
            if (input[i + 1] == '<')
            {
                add_token(&token_list, create_token(HEREDOC, "<<"));
                i++; // skip the second <
            }
            else
                add_token(&token_list, create_token(REDIRECT_IN, "<"));
            start = i + 1;
        }
        else if (input[i] == '>' && !quote)
        {
            if (i > start)
                add_token(&token_list, create_token(WORD, strndup(input + start, i - start)));
            if (input[i + 1] == '>')
            {
                add_token(&token_list, create_token(APPEND, ">>"));
                i++;
            }
            else
                add_token(&token_list, create_token(REDIRECT_OUT, ">"));
            start = i + 1;
        }
        else if (input[i] == '|' && !quote)
        {
            if (last_token_was_pipe)
            {
                fprintf(stderr, "Error: Invalid sequence of consecutive '|' operators\n");
                free_tokens(token_list);
                return (NULL);
            }
            last_token_was_pipe = 1; // set flag for pipe
            if (i > start)
                add_token(&token_list, create_token(WORD, strndup(input + start, i - start)));
            add_token(&token_list, create_token(PIPE, "|"));
            start = i + 1;
        }
        else
            last_token_was_pipe = 0; // reset flag if not a pipe
        i++;
    }

    if (quote) // if quote != 0, means unclosed
    {
        fprintf(stderr, "Error: Unclosed quote '%c'\n", quote);
        free_tokens(token_list);
        return (NULL);
    }

    if (i > start)
        add_token(&token_list, create_token(WORD, strndup(input + start, i - start)));

    return (token_list);
}

// free token list
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

void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("Token: %-10s Type: %d\n", tokens->value, tokens->type);
        tokens = tokens->next;
    }
}

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
// void clear_screen()
// {
//     struct winsize w;
//     int i;
    
//     ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
//     i = 0;
//     while (i < w.ws_row)
//     {
//         write(STDOUT_FILENO, "\n", 1);
//         i++;
//     }
//     write(STDOUT_FILENO, "\033[H", 4);
//     write(STDOUT_FILENO, "\033[J", 3);
// }

   // if (!input)
        // {
        //     printf("\nexit\n");
        //     break ;
        // }
        // if (strcmp(input, "exit") == 0)
        // {
        //     free(input);
        //     printf("exit\n");
        //     break ;
        // }
        // if (strcmp(input, "clear") == 0)
        //     clear_screen();

        // if (strcmp(input, "history -c") == 0)
        //     rl_clear_history();  // Clear the command history
        // else
        //     add_history(input);  // Add the input to history

        // free(input);
