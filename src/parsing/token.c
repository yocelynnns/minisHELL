/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:19:25 by ysetiawa          #+#    #+#             */
/*   Updated: 2025/01/15 01:19:05 by yocelynnns       ###   ########.fr       */
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
	if(!new_token->value)
	{
		free(new_token->value);
		free(new_token);
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

// char    *first_processing(char *str, t_minishell *mini)
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
//         else if (str[i] == '$') {
// 			start = ++i + str;
// 			while (str[i] && !ft_strchr("\\\"\' $", str[i]))
// 				i++;
// 			start = ft_strndup(start, (str + i) - start);
// 			char *env_value = get_env_value(start, mini->env);
// 			if (env_value) {
// 				result = ft_strjoin(result, env_value);
// 			} else {
// 				result = ft_strcjoin(result, '\0'); // Append empty if undefined
// 			}
// 			free(start);
// 		}
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
//                 else if (str[i] == '$')
//                 {
//                     start = ++i + str;
//                     while (str[i] && !ft_strchr("\\\"$", str[i]))
//                         i++;
//                     start = ft_strndup(start, (str + i) - start);
//                     if (start && *start)
//                     {
//                         char *env_val = get_env_value(start, mini->env);
//                         if(env_val == NULL)
//                             env_val = ""; 
//                         result = ft_strjoin(result, env_val);
//                         free(start);
//                     }
//                     else
//                         result = ft_strcjoin(result, str[i - 1]);
//                 }
//                 else
//                 {
//                     while (str[i] && str[i] != '\"')
//                         result = ft_strcjoin(result, str[i++]);
//                     i++;
//                 }
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

// t_token *lexer(const char *input, t_minishell *mini)
// {
//     t_token *token_list = NULL;
//     int i = 0;
//     int start = 0;
//     char quote = 0; // to track quotes
//     int last_token_was_pipe = 0; // to track double pipes
// 	char *status_str;
	
//     while (input[i])
// 	{
// 		if (input[i] == '$' && (input[i - 1] != '=') && (input[i
// 						- 1] != '\'') && (input[i - 2] != '='))
// 		{
// 			if (input[i + 1] == '?')
// 			{
// 				status_str = ft_itoa(g_exit_status);
// 				add_token(&token_list, create_token(WORD, status_str));
// 				free(status_str);
// 				i++;
// 				start = i + 1;
// 			}
// 			else if (isalpha(input[i + 1])) {
//                 // Handle environment variable, e.g., $USER
//                 int var_start = i + 1;
//                 while (input[i + 1] && (isalnum(input[i + 1]) || input[i + 1] == '_')) {
//                     i++;
//                 }
//                 char *var_name = ft_strndup(input + var_start, i + 1 - var_start);
//                 char *var_value = get_env_value(var_name, mini->env);
//                 free(var_name);

//                 // Add token with the environment variable's value
//                 if (var_value) {
//                     add_token(&token_list, create_token(WORD, var_value));
//                     // free(var_value);
//                 } else {
//                     // If no value found, just add the token as is
//                     add_token(&token_list, create_token(WORD, ""));
//                 }
//                 start = i + 1; // Set start to the next character after the variable name
//             }
// 		}
//         else if (input[i] == '\'' || input[i] == '"')
//          {
//             if (quote == 0)
//                 quote = input[i]; // first quote
//             else if (quote == input[i])
//                 quote = 0; // close quote
//         }
//         else if (isspace(input[i]) && !quote) {
//             if (i > start) {
//                 char *raw_token = strndup(input + start, i - start);
//                 char *processed_token = first_processing(raw_token, mini);
//                 add_token(&token_list, create_token(WORD, processed_token));
//                 free(raw_token);
//             }
//             start = i + 1; // move to the next token
//         }
//         else if (input[i] == '<' && !quote) {
//             if (i > start) {
//                 char *raw_token = strndup(input + start, i - start);
//                 char *processed_token = first_processing(raw_token, mini);
//                 add_token(&token_list, create_token(WORD, processed_token));
//                 free(raw_token);
//             }
//             if (input[i + 1] == '<') {
//                 add_token(&token_list, create_token(HEREDOC, "<<"));
//                 i++; // skip the second <
//             } else {
//                 add_token(&token_list, create_token(REDIRECT_IN, "<"));
//             }
//             start = i + 1;
//         }
//         else if (input[i] == '>' && !quote) {
//             if (i > start) {
//                 char *raw_token = strndup(input + start, i - start);
//                 char *processed_token = first_processing(raw_token, mini);
//                 add_token(&token_list, create_token(WORD, processed_token));
//                 free(raw_token);
//             }
//             if (input[i + 1] == '>') {
//                 add_token(&token_list, create_token(APPEND, ">>"));
//                 i++;
//             } else {
//                 add_token(&token_list, create_token(REDIRECT_OUT, ">"));
//             }
//             start = i + 1;
//         }
//         else if (input[i] == '|' && !quote) {
//             if (last_token_was_pipe) {
//                 fprintf(stderr, "Error: Invalid sequence of consecutive '|' operators\n");
//                 free_tokens(token_list);
//                 return NULL;
//             }
//             last_token_was_pipe = 1; // set flag for pipe
//             if (i > start) {
//                 char *raw_token = strndup(input + start, i - start);
//                 char *processed_token = first_processing(raw_token, mini);
//                 add_token(&token_list, create_token(WORD, processed_token));
//                 free(raw_token);
//             }
//             add_token(&token_list, create_token(PIPE, "|"));
//             start = i + 1;
//         }
//         else {
//             last_token_was_pipe = 0; // reset flag if not a pipe
//         }
//         i++;
//     }

//     if (quote) { // if quote != 0, means unclosed
//         fprintf(stderr, "Error: Unclosed quote '%c'\n", quote);
//         free_tokens(token_list);
//         return NULL;
//     }

//     if (i > start) {
//         char *raw_token = strndup(input + start, i - start);
//         char *processed_token = first_processing(raw_token, mini);
//         add_token(&token_list, create_token(WORD, processed_token));
//         free(raw_token);
//     }

//     return token_list;
// }

t_ast_node	*build_ast(t_token *tokens)
{
	return (parse_pipeline(&tokens));
}

void init_lexstate(t_lexer_state *state)
{
    state->i = 0;
	state->start = 0;
	state->quote = 0;
    state->last_token_was_pipe = 0;
	state->token_list = NULL;
}

void checkquote(t_lexer_state *state)
{
	if (state->quote)
	{
		printf("Error: Unclosed quote '%c'\n", state->quote);
		free_tokens(state->token_list);
        return ;
	}
}

void handle_dollar(const char *input, t_lexer_state *state, t_minishell *mini)
{
    if (input[state->i + 1] == '?')
    {
        char *status_str = ft_itoa(g_exit_status);
        add_token(&state->token_list, create_token(WORD, status_str));
        free(status_str);
        state->i++;
        state->start = state->i + 1;
    }
    else if (isalpha(input[state->i + 1]))
    {
        int var_start = state->i + 1;
        while (isalnum(input[state->i + 1]) || input[state->i + 1] == '_')
            state->i++;
        char *var_name = ft_strndup(input + var_start, state->i + 1 - var_start);
        char *var_value = get_env_value(var_name, mini->env);
        free(var_name);
        if (var_value)
            add_token(&state->token_list, create_token(WORD, var_value));
        else
            add_token(&state->token_list, create_token(WORD, ""));
        state->start = state->i + 1;
    }
}

void handle_quotes(const char *input, t_lexer_state *state)
{
    if (state->quote == 0)
        state->quote = input[state->i];
    else if (state->quote == input[state->i])
        state->quote = 0;
}

void handle_spaces(const char *input, t_lexer_state *state, t_minishell *mini)
{
    if (state->i > state->start)
    {
        char *raw_token = strndup(input + state->start, state->i - state->start);
        char *processed_token = first_processing(raw_token, mini);
        add_token(&state->token_list, create_token(WORD, processed_token));
        free(raw_token);
    }
    state->start = state->i + 1;
}

void handle_redir(const char *input, t_lexer_state *state, char direction, t_minishell *mini)
{
    if (state->i > state->start)
    {
        char *raw_token = strndup(input + state->start, state->i - state->start);
        char *processed_token = first_processing(raw_token, mini);
        add_token(&state->token_list, create_token(WORD, processed_token));
        free(raw_token);
    }
    if (direction == '<')
    {
        if (input[state->i + 1] == '<')
        {
            add_token(&state->token_list, create_token(HEREDOC, "<<"));
            state->i++;
        }
        else
            add_token(&state->token_list, create_token(REDIRECT_IN, "<"));
    }
    else if (direction == '>')
    {
        if (input[state->i + 1] == '>')
        {
            add_token(&state->token_list, create_token(APPEND, ">>"));
            state->i++;
        }
        else
            add_token(&state->token_list, create_token(REDIRECT_OUT, ">"));
    }
    state->start = state->i + 1;
}

void handle_pipe(const char *input, t_lexer_state *state, t_minishell *mini)
{
    if (state->last_token_was_pipe)
    {
        fprintf(stderr, "Error: Invalid sequence of consecutive '|' operators\n");
        free_tokens(state->token_list);
        state->token_list = NULL;
    }
    state->last_token_was_pipe = 1;
    if (state->i > state->start)
    {
        char *raw_token = strndup(input + state->start, state->i - state->start);
        char *processed_token = first_processing(raw_token, mini);
        add_token(&state->token_list, create_token(WORD, processed_token));
        free(raw_token);
    }
    add_token(&state->token_list, create_token(PIPE, "|"));
    state->start = state->i + 1;
}

void process_remaining_token(const char *input, t_lexer_state *state, t_minishell *mini)
{
    if (state->i > state->start)
    {
        char *raw_token = strndup(input + state->start, state->i - state->start);
        char *processed_token = first_processing(raw_token, mini);
        if (processed_token)
            add_token(&state->token_list, create_token(WORD, processed_token));
        free(raw_token);
    }
}

t_token *lexer(const char *input, t_minishell *mini)
{
    t_lexer_state state;
    
    init_lexstate(&state);
    while (input[state.i])
    {
        if (input[state.i] == '$' && (input[state.i - 1] != '=') && \
        (input[state.i - 1] != '\'') && (input[state.i - 2] != '='))
            handle_dollar(input, &state, mini);
        else if (input[state.i] == '\'' || input[state.i] == '"')
            handle_quotes(input, &state);
        else if (isspace(input[state.i]) && !state.quote)
            handle_spaces(input, &state, mini);
        else if ((input[state.i] == '<' || input[state.i] == '>') && !state.quote)
            handle_redir(input, &state, input[state.i], mini);
        else if (input[state.i] == '|' && !state.quote)
            handle_pipe(input, &state, mini);
        else
            state.last_token_was_pipe = 0;
        state.i++;
    }
    checkquote(&state);
    process_remaining_token(input, &state, mini);
    return state.token_list;
}

// void add_raw_token(t_lexer_state *state, const char *input, int *i, t_minishell *mini)
// {
//     char *raw_token = strndup(input + state->start, *i - state->start);
//     char *processed_token = first_processing(raw_token, mini);
//     add_token(&state->token_list, create_token(WORD, processed_token));
//     free(raw_token);
// } 

// void checkpipe(const char *input, int *i, t_lexer_state *state)
// {
// 	if (input[*i] == '|')
// 	{
// 		printf("Error: Syntax error near unexpected token `|'\n");
// 		free_tokens(state->token_list);
// 	}
// }

// t_token	*lexer(const char *input, t_minishell *mini)
// {
// 	t_lexer_state	state;
// 	int				i;

// 	i = 0;
// 	init_lexstate(&state);
// 	checkpipe(input, &i, &state);
// 	while (input[i])
// 	{
// 		if (input[i] == '$' && (input[i - 1] != '=') && (input[i
// 						- 1] != '\'') && (input[i - 2] != '='))
// 			handle_variable_expansion(&state, input, &i, mini);
// 		else if (input[i] == '\'' || input[i] == '"' || isspace(input[i]))
// 			state.quote = handle_quotes(input[i], state.quote);
//         else if (isspace(input[i]) && !state.quote)
//             handle_space(&state, input, &i, mini);
// 		else if (input[i] == '<' || input[i] == '>' || input[i] == '|')
// 			handle_special_char(&state, input, &i, mini);
// 		// else if (input[i] != '\0')
// 		// 	state.flag = 1;
// 		i++;
// 	}
// 	checkquote(&state);
// 	if (i > state.start)
// 		add_raw_token(&state, input, &i, mini);
// 	return (state.token_list);
// }

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
