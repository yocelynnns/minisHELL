/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:43:56 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/27 14:26:34 by ysetiawa         ###   ########.fr       */
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

int handle_quotes_spaces(t_lexer_state *state, const char *input, int *i)
{
    if (input[*i] == '\'' || input[*i] == '"')
    {
        char quote_char = input[*i];
        if (state->quote == 0)
            state->quote = quote_char;  // Opening quote
        else if (state->quote == quote_char)
            state->quote = 0;  // Closing quote
        else
            return 1;  // Mismatched quotes

        (*i)++;  // Skip the quote mark
        int temp_index = 0;
        char *quoted_content = malloc(strlen(input) + 1); // Allocate enough space for quoted content
        if (!quoted_content) return 1; // Check for malloc failure

        while (input[*i] && !(input[*i] == quote_char && state->quote == 0))
        {
            if (input[*i] == '\\' && input[*i + 1] == quote_char)
            {
                // Handle escape sequence
                quoted_content[temp_index++] = quote_char;
                (*i) += 2;  // Skip the escape character
            }
            else if (input[*i] == quote_char)
            {
                state->quote = 0; // Closing quote
                break;  // End of quoted content
            }
            else
            {
                quoted_content[temp_index++] = input[*i];
                (*i)++;  // Continue reading content inside quotes
            }
        }
        quoted_content[temp_index] = '\0'; // Null-terminate the quoted content

        // Calculate prefix length correctly
        int prefix_len = *i - state->start - temp_index - 1; // -1 for the opening quote
        char *final_token = malloc(prefix_len + temp_index + 1); // Allocate enough space
        if (!final_token) { free(quoted_content); return 1; } // Check for malloc failure

        ft_strncpy(final_token, (char *)(input + state->start), prefix_len);
        final_token[prefix_len] = '\0'; // Null-terminate the final token
        strcat(final_token, quoted_content); // Concatenate quoted content
        add_token(&state->token_list, create_token(WORD, final_token));
        
        free(quoted_content);
        free(final_token);
        state->start = *i; // Update start position
    }
    else if (isspace(input[*i]) && !state->quote)
    {
        if (*i > state->start)
            add_token(&state->token_list, create_token(WORD, ft_strndup(input + state->start, *i - state->start)));
        state->start = *i + 1;
    }
    return state->quote;
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
        state->start = *i + 1;
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