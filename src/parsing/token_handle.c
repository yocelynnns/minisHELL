/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:43:56 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/12 20:54:05 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// handle special characters (redirects, pipes)
void handle_redirect_in(t_lexer_state *state, const char *input, int *i)
{
    if (input[*i + 1] == '<')
        add_token(&state->token_list, create_token(HEREDOC, "<<")), (*i)++;
    else
        add_token(&state->token_list, create_token(REDIRECT_IN, "<"));
}

void handle_redirect_out(t_lexer_state *state, const char *input, int *i)
{
    if (input[*i + 1] == '>')
        add_token(&state->token_list, create_token(APPEND, ">>")), (*i)++;
    else
        add_token(&state->token_list, create_token(REDIRECT_OUT, ">"));
}

void handle_pipe(t_lexer_state *state)
{
    if (state->last_token_was_pipe)
    {
        fprintf(stderr, "Error: Invalid sequence of consecutive '|' operators\n");
        exit(EXIT_FAILURE);
    }
    state->last_token_was_pipe = 1;
    add_token(&state->token_list, create_token(PIPE, "|"));
}

// general function to handle special characters
void handle_special_char(t_lexer_state *state, const char *input, int *i)
{
    if (input[*i] == '<')
    {
        if (*i > state->start)
            add_token(&state->token_list, create_token(WORD, strndup(input + state->start, *i - state->start)));
        handle_redirect_in(state, input, i);
    }
    else if (input[*i] == '>')
    {
        if (*i > state->start)
            add_token(&state->token_list, create_token(WORD, strndup(input + state->start, *i - state->start)));
        handle_redirect_out(state, input, i);
    }
    else if (input[*i] == '|')
    {
        if (*i > state->start)
            add_token(&state->token_list, create_token(WORD, strndup(input + state->start, *i - state->start)));
        handle_pipe(state);
    }
    state->start = *i + 1;
}

// handle quotes and spaces
void handle_quotes_spaces(t_lexer_state *state, const char *input, int *i)
{
    if (input[*i] == '\'' || input[*i] == '"')
    {
        if (state->quote == 0)
            state->quote = input[*i];
        else if (state->quote == input[*i])
            state->quote = 0;
    }
    else if (isspace(input[*i]) && !state->quote)
    {
        if (*i > state->start)
            add_token(&state->token_list, create_token(WORD, strndup(input + state->start, *i - state->start)));
        state->start = *i + 1;
    }
    else
        state->last_token_was_pipe = 0;
}
