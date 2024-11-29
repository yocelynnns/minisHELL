/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:51:00 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/11/28 17:57:05 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void clear_screen()
{
    struct winsize w;
    int i;
    
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    i = 0;
    while (i < w.ws_row)
    {
        write(STDOUT_FILENO, "\n", 1);
        i++;
    }
    write(STDOUT_FILENO, "\033[H", 4);
    write(STDOUT_FILENO, "\033[J", 3);
}

// Helper function to create a new token
t_token *create_token(t_token_type type, const char *value)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_token->type = type;
    new_token->value = strdup(value);
    new_token->next = NULL;
    return new_token;
}

// Add a token to the end of the token list
void add_token(t_token **head, t_token *new_token)
{
    if (!*head)
        *head = new_token;
    else
    {
        t_token *temp = *head;
        while (temp->next)
            temp = temp->next;
        temp->next = new_token;
    }
}

// Function to tokenize input and assign token types
t_token *lexer(const char *input) {
    t_token *token_list = NULL;
    int i = 0, start = 0;
    char quote = 0; // To track quotes (' or ")

    while (input[i])
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            if (quote == 0)
                quote = input[i]; // Start quote
            else if (quote == input[i])
                quote = 0; // End quote
        }
        else if (isspace(input[i]) && !quote)
        {
            if (i > start) // Add the token
                add_token(&token_list, create_token(WORD, strndup(input + start, i - start)));
            start = i + 1; // Move to the next token
        }
        else if (input[i] == '|' && !quote)
        {
            if (i > start) // Add the previous token
                add_token(&token_list, create_token(WORD, strndup(input + start, i - start)));
            add_token(&token_list, create_token(PIPE, "|"));
            start = i + 1;
        }
        else if (input[i] == '<' && !quote)
        {
            if (i > start) // Add the previous token
                add_token(&token_list, create_token(WORD, strndup(input + start, i - start)));
            if (input[i + 1] == '<')
            {
                add_token(&token_list, create_token(HEREDOC, "<<"));
                i++; // Skip the second '<'
            }
            else
                add_token(&token_list, create_token(REDIRECT_IN, "<"));
            start = i + 1;
        }
        else if (input[i] == '>' && !quote)
        {
            if (i > start) // Add the previous token
                add_token(&token_list, create_token(WORD, strndup(input + start, i - start)));
            if (input[i + 1] == '>')
            {
                add_token(&token_list, create_token(APPEND, ">>"));
                i++; // Skip the second '>'
            }
            else
                add_token(&token_list, create_token(REDIRECT_OUT, ">"));
            start = i + 1;
        }
        
        i++;
    }
    if (i > start) // Add the last token
        add_token(&token_list, create_token(WORD, strndup(input + start, i - start)));

    return token_list;
}

// Free the token list
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

// Debug function to print tokens
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

    while (1)
    {
        input = readline("minishell$ "); // Prompt user for input
        if (!input)
        {
            printf("exit\n");
            break ; // Exit if Ctrl+D is pressed
        }
        // Exit cmd
        if (strcmp(input, "exit") == 0)
        {
            free(input);
            printf("exit\n");
            break ;
        }
        // Clear cmd
        else if (strcmp(input, "clear") == 0)
            clear_screen();
        // Add input to history for readline functionality
        if (*input)
            add_history(input);
        // Tokenize the input
        tokens = lexer(input);
        // Debug print tokens
        // printf("Tokens:\n");
        print_tokens(tokens);
        // Free the tokens after use
        free_tokens(tokens);
        
    }
    return (0);
}

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