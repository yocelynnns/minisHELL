/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:19:25 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/19 19:49:48 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	return (new_token);
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

t_token	*lexer(const char *input)
{
	t_lexer_state	state;
	int				i;

	state.token_list = NULL;
	state.start = 0;
	state.quote = 0;
	i = 0;
	if (input[i] == '|')
	{
		printf("Error: Syntax error near unexpected token `|'\n");
		return (free_tokens(state.token_list), NULL);
	}
	while (input[i])
	{
		if (input[i] == '$' && !state.quote)
            handle_variable_expansion(&state, input, &i);
		else if (input[i] == '\'' || input[i] == '"' || isspace(input[i]))
			handle_quotes_spaces(&state, input, &i);
		else if (input[i] == '<' || input[i] == '>' || input[i] == '|')
			handle_special_char(&state, input, &i);
		i++;
	}
	if (state.quote)
	{
		printf("Error: Unclosed quote '%c'\n", state.quote);
		return (free_tokens(state.token_list), NULL);
	}
	else if (i > state.start)
		add_token(&state.token_list, create_token(WORD, ft_strndup(input
					+ state.start, i - state.start)));
	return (state.token_list);
}
