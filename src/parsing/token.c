/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:19:25 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/11 17:06:20 by ysetiawa         ###   ########.fr       */
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

t_token	*lexer(const char *input)
{
	t_token	*token_list;
	int		i;
	int		start;
	int		last_pipe;
	char	quote;

	i = 0;
	start = 0;
	last_pipe = 0;
	token_list = NULL;
	quote = 0;
	while (input[i])
	{
		handle_quotes(input[i], &quote);
		if (isspace(input[i]) && !quote)
			handle_word(input, &start, i, &token_list);
		handle_redirect(input, &i, &start, &token_list);
		handle_pipe(input, &i, &start, &token_list, &last_pipe);
		i++;
	}
	if (quote)
	{
		fprintf(stderr, "Error: Unclosed quote '%c'\n", quote);
		free_tokens(token_list);
		return (NULL);
	}
	handle_word(input, &start, i, &token_list);
	return (token_list);
}
