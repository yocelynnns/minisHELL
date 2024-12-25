/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:43:56 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/26 00:28:56 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_redirect_in(t_lexer_state *state, const char *input, int *i)
{
	if (input[*i + 1] == '<')
	{
		add_token(&state->token_list, create_token(HEREDOC, "<<"));
		(*i)++;
	}
	else
		add_token(&state->token_list, create_token(REDIRECT_IN, "<"));
	state->start = *i + 1;
}

void	handle_redirect_out(t_lexer_state *state, const char *input, int *i)
{
	if (input[*i + 1] == '>')
	{
		add_token(&state->token_list, create_token(APPEND, ">>"));
		(*i)++;
	}
	else
		add_token(&state->token_list, create_token(REDIRECT_OUT, ">"));
	state->start = *i + 1;
}

void	handle_special_char(t_lexer_state *state, const char *input, int *i)
{
	if (input[*i] == '<' && !state->quote)
	{
		if (*i > state->start)
			add_token(&state->token_list, create_token(WORD, ft_strndup(input
						+ state->start, *i - state->start)));
		handle_redirect_in(state, input, i);
	}
	else if (input[*i] == '>' && !state->quote)
	{
		if (*i > state->start)
			add_token(&state->token_list, create_token(WORD, ft_strndup(input
						+ state->start, *i - state->start)));
		handle_redirect_out(state, input, i);
	}
	else if (input[*i] == '|' && !state->quote)
	{
		if (*i > state->start)
			add_token(&state->token_list, create_token(WORD, ft_strndup(input
						+ state->start, *i - state->start)));
		add_token(&state->token_list, create_token(PIPE, "|"));
		state->start = *i + 1;
	}
}

void    handle_quotes_spaces(t_lexer_state *state, const char *input, int *i)
{
    if (input[*i] == '\'' || input[*i] == '"')
    {
        char quote_char = input[*i];
        if (state->quote == 0)
            state->quote = quote_char;
        else if (state->quote == quote_char)
            state->quote = 0;
        else
            return;
        (*i)++;
        state->start = *i;
        char *temp = malloc(strlen(input) + 1);
        int temp_index = 0;
        while (input[*i])
        {
            if (input[*i] == '\\' && input[*i + 1] == quote_char)
            {
                temp[temp_index++] = quote_char;
                (*i) += 2;
            }
            else if (input[*i] == quote_char)
                break;
            else
            {
                temp[temp_index++] = input[*i];
                (*i)++;
            }
        }
        temp[temp_index] = '\0';
        if (input[*i] == quote_char)
        {
            add_token(&state->token_list, create_token(WORD, ft_strdup(temp)));
            state->quote = 0;
        }
        free(temp);
        state->start = *i + 1;
    }
    else if (isspace(input[*i]) && !state->quote)
    {
        if (*i > state->start)
            add_token(&state->token_list, create_token(WORD, ft_strndup(input + state->start, *i - state->start)));
        state->start = *i + 1;
    }
}

void handle_variable_expansion(t_lexer_state *state, const char *input, int *i)
{
    char *status_str;

    if (input[*i + 1] == '?')
    {
        status_str = ft_itoa(g_exit_status);
        add_token(&state->token_list, create_token(WORD, status_str));
        free(status_str);
        (*i)++;
    }
    else
        return ;    
}

    // int var_start;
    // char *var_name;
    // char *var_value;
// else if (input[*i + 1] == '\0' || isspace(input[*i + 1]))
//         add_token(&state->token_list, create_token(WORD, ft_strdup("$")));
//     else
//     {
//         var_start = *i + 1;
//         while (ft_isalnum(input[*i + 1]) || input[*i + 1] == '_')
//             (*i)++;
//         var_name = ft_strndup(input + var_start, *i - var_start + 1);
//         var_value = get_env_value(var_name, mini->env);
//         if (var_value)
//             add_token(&state->token_list, create_token(WORD, ft_strdup(var_value)));
//         else
//             add_token(&state->token_list, create_token(WORD, ft_strdup("")));
//         free(var_name);
//     }
//     state->start = *i + 1;