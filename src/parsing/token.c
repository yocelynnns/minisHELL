/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:19:25 by ysetiawa          #+#    #+#             */
/*   Updated: 2025/02/22 00:54:11 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	checkquote(t_lexer_state *state, t_minishell *mini)
{
	if (state->quote)
	{
		printf("Error: Unclosed quote '%c'\n", state->quote);
		free_tokens(state->token_list);
		mini->exit = 2;
		return (1);
	}
	return (0);
}

void	handle_quotes(const char *input, t_lexer_state *state)
{
	if (state->quote == 0)
		state->quote = input[state->i];
	else if (state->quote == input[state->i])
		state->quote = 0;
}

void	handle_spaces(const char *input, t_lexer_state *state, \
	t_minishell *mini)
{
	char	*raw_token;
	char	*processed_token;

	if (state->i > state->start)
	{
		raw_token = ft_strndup(input + state->start, state->i - state->start);
		processed_token = first_processing(raw_token, mini);
		add_token(&state->token_list, create_token(WORD, processed_token));
		free(raw_token);
		free(processed_token);
	}
	state->start = state->i + 1;
}

int	handle_pipe(const char *input, t_lexer_state *state, t_minishell *mini)
{
	char	*raw_token;
	char	*processed_token;

	if (state->last_token_was_pipe)
	{
		fprintf(stderr,
			"Error: Invalid sequence of consecutive '|' operators\n");
		mini->exit = 2;
		return (1);
	}
	state->last_token_was_pipe = 1;
	if (state->i > state->start)
	{
		raw_token = ft_strndup(input + state->start, state->i - state->start);
		processed_token = first_processing(raw_token, mini);
		add_token(&state->token_list, create_token(WORD, processed_token));
		free(raw_token);
		free(processed_token);
	}
	add_token(&state->token_list, create_token(PIPE, "|"));
	state->start = state->i + 1;
	return (0);
}

void	dollar_expan(char *processed_t, t_lexer_state *state)
{
	int		i;
	char	**split_tokens;

	i = 0;
	split_tokens = NULL;
	split_tokens = ft_split(processed_t, ' ');
	free(processed_t);
	if (!split_tokens)
		return ;
	i = 0;
	while (split_tokens[i])
	{
		add_token(&state->token_list, create_token(WORD, split_tokens[i]));
		free(split_tokens[i]);
		i++;
	}
	free(split_tokens);
}

	// if (input[state->i] == '$')
	// 	handle_dollar(input, state, mini);
// void	handle_dollar(const char *input, t_lexer_state *state,
// 		t_minishell *mini)
// {
// 	// char	*status_str;
// 	char	*empty;

// 	(void)mini;
	// if (input[state->i + 1] == '?')
	// {
	// 	status_str = ft_itoa(mini->exit);
	// 	add_token(&state->token_list, create_token(WORD, status_str));
	// 	free(status_str);
	// 	state->i++;
	// 	state->start = state->i + 1;
	// }
	// if (input[state->i + 1] == '\0' && input[state->i - 1] != ' ')
	// {
	// 	empty = "$";
	// 	add_token(&state->token_list, create_token(WORD, empty));
	// 	state->start = state->i + 1;
	// }
// }
		// if (input[state.i] == '$')
		// 	handle_dollar(input, &state, mini);
		// else if ((input[state.i] == '\'' || input[state.i] == '"')
		// && (state.i == 0 || input[state.i - 1] != '\\'))
	// 	handle_quotes(input, &state);
		// else if (isspace(input[state.i]) && !state.quote)
		// 	handle_spaces(input, &state, mini);
		// else if ((input[state.i] == '<' || input[state.i] == '>')
		// 	&& !state.quote)
		// 	handle_redir(input, &state, input[state.i], mini);
		// else if (input[state.i] == '|' && !state.quote)
		// 	handle_pipe(input, &state, mini);
		// else
		// 	state.last_token_was_pipe = 0;

//   if (input[state.i] == '$' && (input[state.i - 1] != '=') &&
//         (input[state.i - 1] != '\'') && (input[state.i - 2] != '=')
	// && (input[state.i - 1] != '\\'))
