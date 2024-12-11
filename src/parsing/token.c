/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:19:25 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/11 20:27:07 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// create a new token
t_token	*create_token(t_token_type type, const char *value)
{
	t_token	*new_token;

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
void	add_token(t_token **head, t_token *new_token)
{
	t_token	*temp;

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
