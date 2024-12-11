/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:05:53 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/11 17:28:11 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	handle_quotes(char c, char *quote)
{
	if (c == '\'' || c == '"')
	{
		if (*quote == 0)
			*quote = c;
		else if (*quote == c)
			*quote = 0;
		return (1);
	}
	return (0);
}

void	handle_word(const char *input, int *start, \
int i, t_token **token_list)
{
	if (i > *start)
	{
		add_token(token_list, create_token(WORD, \
		strndup(input + *start, i - *start)));
		*start = i + 1;
	}
}

int	handle_redirect(const char *input, int *i, \
int *start, t_token **token_list)
{
	int	redirect_type;

	if (input[*i] == '<' || input[*i] == '>')
	{
		handle_word(input, start, *i, token_list);
		if (input[*i] == '<' && input[*i + 1] == '<')
		{
			add_token(token_list, create_token(HEREDOC, "<<"));
			(*i)++;
		}
		else if (input[*i] == '>' && input[*i + 1] == '>')
		{
			add_token(token_list, create_token(APPEND, ">>"));
			(*i)++;
		}
		else
		{
			if (input[*i] == '<')
				redirect_type = REDIRECT_IN;
			else
				redirect_type = REDIRECT_OUT;
			add_token(token_list, create_token(redirect_type, \
			strndup(input + *i, 1)));
		}
		*start = *i + 1;
		return (1);
	}
	return (0);
}

int	handle_pipe(const char *input, int *i, int \
*start, t_token **token_list, int *last_pipe)
{
	if (input[*i] == '|')
	{
		if (*last_pipe)
		{
			fprintf(stderr, "Error: Invalid \
			sequence of consecutive '|' operators\n");
			free_tokens(*token_list);
			return (0);
		}
		*last_pipe = 1;
		handle_word(input, start, *i, token_list);
		add_token(token_list, create_token(PIPE, "|"));
		*start = *i + 1;
		return (1);
	}
	*last_pipe = 0;
	return (0);
}
