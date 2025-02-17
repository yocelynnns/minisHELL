/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 00:01:29 by yocelynnns        #+#    #+#             */
/*   Updated: 2025/02/18 00:08:41 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_redirect_in(t_lexer_state *state, const char *input, \
	t_minishell *mini)
{
	if (input[state->i + 1] == '<')
	{
		mini->here = 1;
		add_token(&state->token_list, create_token(HEREDOC, "<<"));
		state->i++;
	}
	else
		add_token(&state->token_list, create_token(REDIRECT_IN, "<"));
}

void	handle_redirect_out(t_lexer_state *state, const char *input)
{
	if (input[state->i + 1] == '>')
	{
		add_token(&state->token_list, create_token(APPEND, ">>"));
		state->i++;
	}
	else
		add_token(&state->token_list, create_token(REDIRECT_OUT, ">"));
}

void	handle_redir(const char *input, t_lexer_state *state, char direction, \
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
	if (direction == '<')
		handle_redirect_in(state, input, mini);
	else if (direction == '>')
		handle_redirect_out(state, input);
	state->start = state->i + 1;
}

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

t_ast_node	*build_ast(t_token *tokens, t_minishell *mini)
{
	int	i;

	i = count_cmds(tokens);
	return (parse_pipeline(tokens, mini, i));
}
