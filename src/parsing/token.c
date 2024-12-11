/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:19:25 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/11 19:06:49 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	handle_quotes(char c, char *quote)
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

static void	handle_word(const char *input, int *start, \
int i, t_token **token_list)
{
	if (i > *start)
	{
		add_token(token_list, create_token(WORD, \
		strndup(input + *start, i - *start)));
		*start = i + 1;
	}
}

static int	handle_redirect(const char *input, int *i, \
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

static int	handle_pipe(const char *input, int *i, int \
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
