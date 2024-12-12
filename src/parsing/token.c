/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:19:25 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/12 17:45:23 by ysetiawa         ###   ########.fr       */
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

// main lexer function
t_token *lexer(const char *input)
{
    t_lexer_state state = {NULL, 0, 0, 0};
    int i = 0;

    while (input[i])
    {
        if (input[i] == '\'' || input[i] == '"' || isspace(input[i]))
            handle_quotes_spaces(&state, input, &i);
        else if (input[i] == '<' || input[i] == '>' || input[i] == '|')
            handle_special_char(&state, input, &i);
        i++;
    }
    if (state.quote)
    {
        fprintf(stderr, "Error: Unclosed quote '%c'\n", state.quote);
        exit(EXIT_FAILURE);
    }
    if (i > state.start)
        add_token(&state.token_list, create_token(WORD, strndup(input + state.start, i - state.start)));
    return state.token_list;
}
