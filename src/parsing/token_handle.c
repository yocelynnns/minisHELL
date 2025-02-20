/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:43:56 by ysetiawa          #+#    #+#             */
/*   Updated: 2025/02/21 01:59:22 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	handle_pipe_error(t_token *tokens, t_minishell *mini)
{
	if (tokens && tokens->next && tokens->type == WORD && tokens->next->type == PIPE)
	{
		if (tokens->next->next == NULL || tokens->next->next->type == PIPE)
		{
			printf("Error: Syntax error near unexpected token '|'\n");
			mini->exit = 2;
			return (0);
		}
	}
	return (1);
}

static t_ast_node	*create_pipeline_node(t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*pipeline_node;

	pipeline_node = create_ast_node(AST_PIPELINE);
	pipeline_node->pipeline = malloc(sizeof(t_ast_pipeline));
	pipeline_node->pipeline->left = left;
	pipeline_node->pipeline->right = right;
	return (pipeline_node);
}

t_ast_node	*parse_pipeline(t_token *tokens, t_minishell *mini, int i)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_ast_node	*pipeline_node;

	if (!handle_pipe_error(tokens, mini))
		return (NULL);
	left = parse_command(&tokens, mini, i);
	if (!left)
		return (NULL);
	if ((tokens) && tokens->type == PIPE)
		right = parse_pipeline(tokens->next, mini, i);
	else
		return (left);
	if (!right)
	{
		free_ast(left);
		return (NULL);
	}
	pipeline_node = create_pipeline_node(left, right);
	left = pipeline_node;
	return (left);
}

char	*ft_strcjoin(char *str, char c)
{
	char	*result;
	int		i;

	result = malloc(sizeof(char) * (ft_strlen(str) + 2));
	i = 0;
	while (str && str[i])
	{
		result[i] = str[i];
		i++;
	}
	result[i++] = c;
	result[i] = '\0';
	free(str);
	return (result);
}

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
	if (!new_token->value)
	{
		free_tokens(new_token);
		return (NULL);
	}
	new_token->next = NULL;
	return (new_token);
}

// void handle_variable_expansion(t_lexer_state *state, const char 
//*input, int *i, t_minishell *mini)
// {
//     char *status_str;
// 	char *var_name;
// 	char *var_value;
// 	int var_start;

// 	if (input[*i + 1] == '?')
// 	{
// 		status_str = ft_itoa(g_exit_status);
// 		add_token(&state->token_list, create_token(WORD, status_str));
// 		free(status_str);
// 		(*i)++;
// 		state->start = *i + 1;
// 	}
// 	else if (isalpha(input[*i + 1]))
// 	{
// 		var_start = *i + 1;
// 		while (input[*i + 1] && (isalnum(input[*i + 1]) ||
// input[*i + 1] == '_'))
// 			(*i)++;
// 		var_name = ft_strndup(input + var_start, *i + 1 - var_start);
// 		var_value = get_env_value(var_name, mini->env);
// 		free(var_name);
// 		if (var_value)
// 			add_token(&state->token_list, create_token(WORD, var_value));
// 		else
// 			add_token(&state->token_list, create_token(WORD, ""));
// 		state->start = *i + 1;
// 	}
// }

// void	handle_special_char(t_lexer_state *state, const char *input,
// int *i, t_minishell *mini)
// {
// 	if (input[*i] == '<' && !state->quote)
// 	{
// 		if (*i > state->start)
// 			add_raw_token(state, input, i, mini);
// 		handle_redirect_in(state, input, i);
// 	}
// 	else if (input[*i] == '>' && !state->quote)
// 	{
// 		if (*i > state->start)
// 			add_raw_token(state, input, i, mini);
// 		handle_redirect_out(state, input, i);
// 	}
// 	else if (input[*i] == '|' && !state->quote)
// 	{
// 		if (*i > state->start)
// 			add_raw_token(state, input, i, mini);
// 		add_token(&state->token_list, create_token(PIPE, "|"));
// 		state->start = *i + 1;
// 	}
// }

// if (input[*i + 1] == '?')
// {
//     status_str = ft_itoa(g_exit_status);
//     add_token(&state->token_list, create_token(WORD, status_str));
//     free(status_str);
//     (*i)++;
//     state->start = *i + 1;
// }
// else
//     return ;
