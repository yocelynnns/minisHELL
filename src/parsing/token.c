/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:19:25 by ysetiawa          #+#    #+#             */
/*   Updated: 2025/01/09 21:35:14 by ysetiawa         ###   ########.fr       */
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
	if(! new_token->value)
	{
		free(new_token->value);
		free_tokens(new_token);
		return NULL;
	}
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

void checkpipe(const char *input, int *i, t_lexer_state *state)
{
	if (input[*i] == '|')
	{
		printf("Error: Syntax error near unexpected token `|'\n");
		free_tokens(state->token_list);
	}
}

void init_lexstate(t_lexer_state *state)
{
	state->token_list = NULL;
	state->start = 0;
	state->quote = 0;
	state->flag = 0;
	state->escquo = 0;
}

void checkquote(t_lexer_state *state)
{
	if (state->quote)
	{
		printf("Error: Unclosed quote '%c'\n", state->quote);
		free_tokens(state->token_list);
	}
}

// void handle_expansion(t_lexer_state *state, const char *input, int *i, t_minishell *mini)
// {
// 	int var_start;
// 	char *var_name;
// 	char *var_value;
	
// 	if (input[*i - 1] == '\'')
// 		return ;
// 	else
// 	{
// 		var_start = *i + 1;
// 		// while (ft_isdigit(input[*i + 1]) || input[*i + 1] == '_')
//         // 	(*i)++;
// 		var_name = ft_strndup(input + var_start, *i - var_start + 1);
// 		var_value = get_env_value(var_name, mini->env);
// 		if (var_value)
// 			add_token(&state->token_list, create_token(WORD, ft_strdup(var_value)));
// 		free(var_name);
// 	}
// 	state->start = *i + 1;
// }

t_token	*lexer(const char *input, t_minishell *mini)
{
	t_lexer_state	state;
	int				i;

	i = 0;
	init_lexstate(&state);
	checkpipe(input, &i, &state);
	while (input[i])
	{
		// if (input[i] == '$' && ((input[i + 1] >= 'A' && input[i + 1] <= 'Z')))
		// 	handle_expansion(&state, input, &i, mini);
		if (input[i] == '$' && (!(input[i - 1] == '=')) && (!(input[i
						- 1] >= 'a' && input[i - 1] <= 'z')))
			handle_variable_expansion(&state, input, &i);
		else if (input[i] == '\'' || input[i] == '"' || isspace(input[i]))
			state.flag = handle_quotes_spaces(&state, input, &i);
		else if (input[i] == '<' || input[i] == '>' || input[i] == '|')
			handle_special_char(&state, input, &i);
		else if (input[i] != '\0')
			state.flag = 1;
		i++;
	}
	checkquote(&state);
	if (state.flag && i > state.start)
		add_token(&state.token_list, create_token(WORD, ft_strndup(input
					+ state.start, i - state.start)));
	return (state.token_list);
}

t_ast_node	*build_ast(t_token *tokens)
{
	return (parse_pipeline(&tokens));
}
