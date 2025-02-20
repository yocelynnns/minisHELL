/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 00:02:48 by yocelynnns        #+#    #+#             */
/*   Updated: 2025/02/21 03:13:19 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	process_remaining_token(const char *input, t_lexer_state *state, \
	t_minishell *mini)
{
	char	*raw_token;
	char	*processed_t;

	raw_token = NULL;
	processed_t = NULL;
	if (state->i > state->start)
	{
		raw_token = ft_strndup(input + state->start, state->i - state->start);
		processed_t = first_processing(raw_token, mini);
		if (input[0] == '$')
			dollar_expan(processed_t, state);
		else
		{
			if (processed_t)
			{
				add_token(&state->token_list, create_token(WORD, processed_t));
				free(processed_t);
			}
		}
		free(raw_token);
	}
}

void	init_lexstate(t_lexer_state *state)
{
	state->i = 0;
	state->start = 0;
	state->quote = 0;
	state->last_token_was_pipe = 0;
	state->token_list = NULL;
}

int	checkpipe(const char *input, t_lexer_state *state, t_minishell *mini)
{
	if (input[state->i] == '|' && input[state->i + 1] != '|')
	{
		printf("Error: Syntax error near unexpected token `|'\n");
		mini->exit = 2;
		free_tokens(state->token_list);
		return (1);
	}
	if (((input[state->i] == '"' && input[state->i + 1] == '"') || \
	(input[state->i] == '\'' && input[state->i + 1] == '\'')) && \
	(input[state->i + 2] == '\0'))
	{
		printf("Command not found: ''\n");
		mini->exit = 127;
		free_tokens(state->token_list);
		return (1);
	}
	return (0);
}

int	lexer_checks(const char *input, t_lexer_state *state, t_minishell *mini)
{
	if ((input[state->i] == '\'' || input[state->i] == '"') && \
	(state->i == 0 || input[state->i - 1] != '\\'))
		return (handle_quotes(input, state), 0);
	else if (isspace(input[state->i]) && !state->quote)
		return(handle_spaces(input, state, mini), 0);
	else if ((input[state->i] == '<' || input[state->i] == '>')
		&& !state->quote)
		return (handle_redir(input, state, input[state->i], mini), 0);
	else if (input[state->i] == '|' && !state->quote)
	{
		if (handle_pipe(input, state, mini) == 1)
			return (1);
		return (0);
	}
	else
		state->last_token_was_pipe = 0;
	return (0);
}

t_token	*lexer(const char *input, t_minishell *mini)
{
	t_lexer_state	state;

	init_lexstate(&state);
	if (checkpipe(input, &state, mini) == 1)
		return (NULL);
	while (input[state.i])
	{
		if (lexer_checks(input, &state, mini) == 1)
			return(free_tokens(state.token_list), NULL);
		state.i++;
	}
	if (checkquote(&state, mini) == 1)
		return (NULL);
	process_remaining_token(input, &state, mini);
	mini->here = 0;
	return (state.token_list);
}
