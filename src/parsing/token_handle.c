/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:43:56 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/20 16:55:19 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_redirect_in(t_lexer_state *state, const char *input, int *i)
{
	if (input[*i + 1] == '<')
	{
		add_token(&state->token_list, create_token(HEREDOC, "<<"));
		(*i)++;
	}
	else
		add_token(&state->token_list, create_token(REDIRECT_IN, "<"));
	state->start = *i + 1;
}

void	handle_redirect_out(t_lexer_state *state, const char *input, int *i)
{
	if (input[*i + 1] == '>')
	{
		add_token(&state->token_list, create_token(APPEND, ">>"));
		(*i)++;
	}
	else
		add_token(&state->token_list, create_token(REDIRECT_OUT, ">"));
	state->start = *i + 1;
}

void	handle_special_char(t_lexer_state *state, const char *input, int *i)
{
	if (input[*i] == '<' && !state->quote)
	{
		if (*i > state->start)
			add_token(&state->token_list, create_token(WORD, ft_strndup(input
						+ state->start, *i - state->start)));
		handle_redirect_in(state, input, i);
	}
	else if (input[*i] == '>' && !state->quote)
	{
		if (*i > state->start)
			add_token(&state->token_list, create_token(WORD, ft_strndup(input
						+ state->start, *i - state->start)));
		handle_redirect_out(state, input, i);
	}
	else if (input[*i] == '|' && !state->quote)
	{
		if (*i > state->start)
			add_token(&state->token_list, create_token(WORD, ft_strndup(input
						+ state->start, *i - state->start)));
		add_token(&state->token_list, create_token(PIPE, "|"));
		state->start = *i + 1;
	}
}

void	handle_quotes_spaces(t_lexer_state *state, const char *input, int *i)
{
	if (input[*i] == '\'' || input[*i] == '"')
	{
		char quote_char = input[*i];
		if (state->quote == 0)
			state->quote = quote_char;
		else if (state->quote == quote_char)
			state->quote = 0;
		else
			return;
		if (state->quote == quote_char)
			state->start++;
		(*i)++;
		while (input[*i] && input[*i] != quote_char)
			(*i)++;
		if (input[*i] == quote_char)
		{
			add_token(&state->token_list, create_token(WORD, \
			ft_strndup(input + state->start, *i - state->start)));
			state->quote = 0;
		}
		state->start = *i + 1;
	}
	else if (isspace(input[*i]) && !state->quote)
	{
		if (*i > state->start)
			add_token(&state->token_list, create_token(WORD, ft_strndup(input + state->start, *i - state->start)));
		state->start = *i + 1;
	}
}

void handle_variable_expansion(t_lexer_state *state, const char *input, int *i)
{
    char *status_str;
    int var_start;
    char *var_name;
    char *var_value;

    if (input[*i + 1] == '?')
    {
        status_str = ft_itoa(state->exit_status);
        add_token(&state->token_list, create_token(WORD, status_str));
        free(status_str);
        (*i)++;
    }
    else if (input[*i + 1] == '\0' || isspace(input[*i + 1]))
        add_token(&state->token_list, create_token(WORD, ft_strdup("$")));
    else
    {
        var_start = *i + 1;
        while (ft_isalnum(input[*i + 1]) || input[*i + 1] == '_')
            (*i)++;
        var_name = ft_strndup(input + var_start, *i - var_start + 1);
        var_value = getenv(var_name);
        if (var_value)
            add_token(&state->token_list, create_token(WORD, ft_strdup(var_value)));
        else
            add_token(&state->token_list, create_token(WORD, ft_strdup("")));
        free(var_name);
    }
    state->start = *i + 1;
}
