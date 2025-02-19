/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 00:02:48 by yocelynnns        #+#    #+#             */
/*   Updated: 2025/02/19 16:28:30 by hthant           ###   ########.fr       */
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

void	lexer_checks(const char *input, t_lexer_state *state, t_minishell *mini)
{
	if ((input[state->i] == '\'' || input[state->i] == '"') && \
	(state->i == 0 || input[state->i - 1] != '\\'))
		handle_quotes(input, state);
	else if (isspace(input[state->i]) && !state->quote)
		handle_spaces(input, state, mini);
	else if ((input[state->i] == '<' || input[state->i] == '>')
		&& !state->quote)
		handle_redir(input, state, input[state->i], mini);
	else if (input[state->i] == '|' && !state->quote)
		handle_pipe(input, state, mini);
	else
		state->last_token_was_pipe = 0;
}

t_token	*lexer(const char *input, t_minishell *mini)
{
	t_lexer_state	state;
	int				j;
	int				i;

	j = 0;
	i = 0;
	init_lexstate(&state);
	i = checkpipe(input, &state, mini);
	while (input[state.i])
	{
		lexer_checks(input, &state, mini);
		state.i++;
	}
	if (mini->exit == 217)
	{
		mini->exit = 2;
		return (NULL);
	}
	j = checkquote(&state, mini);
	if (j || i)
		return (NULL);
	process_remaining_token(input, &state, mini);
	mini->here = 0;
	return (state.token_list);
}
