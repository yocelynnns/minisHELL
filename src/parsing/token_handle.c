/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:43:56 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/18 15:41:58 by ysetiawa         ###   ########.fr       */
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

void	handle_pipe(const char *input, int *i)
{
	if (input[*i + 1] == '|')
	{
		printf("Error: Syntax error near unexpected token '|a|'\n");
		return ;
	}
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
		if (state->quote == 0)
			state->quote = input[*i];
		else if (state->quote == input[*i])
			state->quote = 0;
	}
	else if (isspace(input[*i]) && !state->quote)
	{
		if (*i > state->start)
			add_token(&state->token_list, create_token(WORD, ft_strndup(input
						+ state->start, *i - state->start)));
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
		status_str = getenv("?");
		if (status_str)
            add_token(&state->token_list, create_token(WORD, ft_strdup(status_str)));
        else
            add_token(&state->token_list, create_token(WORD, ft_strdup("0")));
		(*i)++;
	}
	else
	{
		var_start = *i + 1;
		while (ft_isdigit(input[*i + 1]) || input[*i + 1] == '_')
        	(*i)++;
		var_name = ft_strndup(input + var_start, *i - var_start + 1);
		var_value = getenv(var_name);
		if (var_value) // If variable exists, add it as a token
			add_token(&state->token_list, create_token(WORD, ft_strdup(var_value)));
		free(var_name);
	}
	state->start = *i + 1;
}
