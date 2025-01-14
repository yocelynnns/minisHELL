/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlequote.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 19:47:30 by ysetiawa          #+#    #+#             */
/*   Updated: 2025/01/14 21:24:54 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void handle_space(t_lexer_state *state, const char *input, int *i, t_minishell *mini)
{
    if (*i > state->start)
        add_raw_token(state, input, i, mini);
    state->start = *i + 1;
}

// static int handle_quote_open(t_lexer_state *state, char quote_char)
// {
//     if (state->quote == 0)
//         state->quote = quote_char;
//     else if (state->quote == quote_char)
//         state->quote = 0;
//     else
//         return (1);
//     return (0);
// }

// static char *extract_quoted_content(const char *input, t_lexer_state *state, char quote_char, int *i)
// {
//     int temp_index;
//     char *quoted_content;
    
//     temp_index = 0;
//     quoted_content = malloc(ft_strlen(input) + 1);
//     while (input[*i] && !(input[*i] == quote_char && state->quote == 0))
//     {
//         if (input[*i] == '\\' && input[*i + 1] == quote_char)
//         {
//             state->escquo = 1;
//             quoted_content[temp_index++] = quote_char;
//             (*i) += 2;
//         }
//         else if (input[*i] == quote_char)
//         {
//             state->quote = 0;
//             break ;
//         }
//         else
//             quoted_content[temp_index++] = input[(*i)++];
//     }
//     quoted_content[temp_index] = '\0';
//     return (quoted_content);
// }

// static char *create_final_token(const char *input, t_lexer_state *state, char *quoted_content, int *i)
// {
//     int prefix_len;
//     char *final_token;
    
//     if (!state->escquo)
//         prefix_len = *i - state->start - ft_strlen(quoted_content) - 1;
//     else
//         prefix_len = *i - state->start - ft_strlen(quoted_content) - 3;
//     final_token = malloc(prefix_len + ft_strlen(quoted_content) + 1);
//     if (!final_token)
//     {
//         free(quoted_content);
//         return (NULL);
//     }
//     ft_strncpy(final_token, (char *)(input + state->start), prefix_len);
//     final_token[prefix_len] = '\0';
//     ft_strcat(final_token, quoted_content);
//     return (final_token);
// }

char handle_quotes(char current_quote, char quote)
{
    if (quote == 0)
        return current_quote;
    else if (quote == current_quote)
        return 0;
    return quote;
}

// int handle_quotes_spaces(t_lexer_state *state, const char *input, int *i)
// {
//     char quote_char;
//     char *quoted_content;
//     char *final_token;

//     if (input[*i] == '\'' || input[*i] == '"')
//     {
//         quote_char = input[*i];
//         if (handle_quote_open(state, quote_char) != 0)
//             return (1);
//         (*i)++;
//         quoted_content = extract_quoted_content(input, state, quote_char, i);
//         if (!quoted_content)
//             return (1);
//         final_token = create_final_token(input, state, quoted_content, i);
//         free(quoted_content);
//         if (!final_token)
//             return (1);
//         add_token(&state->token_list, create_token(WORD, final_token));
//         free(final_token);
//         state->start = *i + 1;
//     }
//     else if (isspace(input[*i]) && !state->quote)
//         handle_space(state, input, i);
//     return (state->quote);
// }

// char    *ft_strcjoin(char *str, char c)
// {
//     char    *result;
//     int     i;
//     result = malloc(sizeof(char) * (ft_strlen(str) + 2));
//     i = 0;
//     while (str && str[i])
//     {
//         result[i] = str[i];
//         i++;
//     }
//     result[i++] = c;
//     result[i] = '\0';
//     free(str);
//     return (result);
// }
// char    *first_processing(char *str)
// {
//     char    *result;
//     int     i;
//     int     in_quote;
//     char    *start;
//     i = 0;
//     in_quote = 0;
//     result = NULL;
//     while (str[i])
//     {
//         if (str[i] == '\\' && !in_quote)
//         {
//             if (str[i + 1] == '\\' || str[i + 1] == '\'' 
//             || str[i + 1] == '$' || str[i + 1] == '\"')
//             {
//                 result = ft_strcjoin(result, str[++i]);
//                 i++;
//             }
//             else if (str[i + 1] == '\n')
//                 i += 2;
//             else
//                 result = ft_strcjoin(result, str[i++]);
//         }
//         else if (str[i] == '\'')
//         {
//             i++;
//             while (str[i] && str[i] != '\'')
//                 result = ft_strcjoin(result, str[i++]);
//             i++;
//         }
//         else if (str[i] == '\"')
//         {
//             in_quote = str[i++];
//             while (str[i] && in_quote)
//             {
//                 if (str[i] == '\\')
//                 {
//                     if (str[i + 1] == '\\' || 
//                     str[i + 1] == '$' || str[i + 1] == '\"')
//                     {
//                         result = ft_strcjoin(result, str[++i]);
//                         i++;
//                     }
//                     else if (str[i + 1] == '\n')
//                         i += 2;
//                     else
//                         result = ft_strcjoin(result, str[i++]);
//                 }
//                 else
//                     result = ft_strcjoin(result, str[i++]);
//                 if (str[i] == in_quote)
//                 {
//                     in_quote = 0;
//                     i++;
//                 }
//             }
//         }
//         else
//             result = ft_strcjoin(result, str[i++]);
//     }
//     return (result);
// }
