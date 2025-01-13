/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:43:56 by ysetiawa          #+#    #+#             */
/*   Updated: 2025/01/13 21:26:04 by ysetiawa         ###   ########.fr       */
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

void handle_variable_expansion(t_lexer_state *state, const char *input, int *i)
{
    char *status_str;

    if (input[*i + 1] == '?')
    {
        status_str = ft_itoa(g_exit_status);
        add_token(&state->token_list, create_token(WORD, status_str));
        free(status_str);
        (*i)++;
        state->start = *i + 1;
    }
    else
        return ;
}
