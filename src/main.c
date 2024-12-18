/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:51:00 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/18 15:27:18 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int exit_status = 0;

// Function to get the value of a variable
char *get_variable_value(const char *var_name) {
    if (strcmp(var_name, "?") == 0) {
        // Return the exit status as a string
        char *status_str = malloc(12); // Enough for an int
        snprintf(status_str, 12, "%d", exit_status);
        return status_str;
    }
    // Handle other variables as needed
    return NULL; // Or return the value of the variable from the environment
}

void handle_variable_expansion(t_lexer_state *state, const char *input, int *i) {
    int start = *i + 1; // Start after the '$'
    if (input[*i + 1] == '?') {
        // Handle the special case for $?
        (*i)++; // Move to the '?'
        char *var_value = get_variable_value("?");
        if (var_value) {
            add_token(&state->token_list, create_token(WORD, var_value));
        }
        return; // Exit the function after handling $?
    }

    // Handle other variable names
    while (isalnum(input[*i]) || input[*i] == '_') {
        (*i)++;
    }
    int length = *i - start;
    if (length > 0) {
        char *var_name = strndup(input + start, length);
        char *var_value = get_variable_value(var_name);
        if (var_value) {
            add_token(&state->token_list, create_token(WORD, var_value));
        }
        free(var_name);
    }
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_token		*tokens;
	t_ast_node	*ast;
	t_minishell	mini;
	int			exit_status;

	(void)ac;
	(void)av;

	// Initialize environment variables
	if (env_init(&mini, env))
	{
		fprintf(stderr, "Error: Failed to initialize environment variables\n");
		return (EXIT_FAILURE);
	}
	{
		printf(" __   __  ___   __    _  ___   _______  __   __  _______  ___      ___     \n|  |_|  ||   | |  |  | ||   | |       ||  | |  ||       ||   |    |   |    \n|       ||   | |   |_| ||   | |  _____||  |_|  ||    ___||   |    |   |    \n|       ||   | |       ||   | | |_____ |       ||   |___ |   |    |   |    \n|       ||   | |  _    ||   | |_____  ||       ||    ___||   |___ |   |___ \n| ||_|| ||   | | | |   ||   |  _____| ||   _   ||   |___ |       ||       |\n|_|   |_||___| |_|  |__||___| |_______||__| |__||_______||_______||_______|\n");
	}
	// print_sorted_env(mini.env);
	init_signals();
	while (1)
	{
		// Read input from the user
		input = readline("minishell$ ");
		handle_eof(input);
		if (!input)
		{
			printf("exit\n");
			break ;
		}

		// Add input to history if it's not empty
		if (*input)
			add_history(input);

		// Tokenize the input
		tokens = lexer(input);
		if (!tokens)
		{
			free(input);
			continue ;
		}

		// Build the abstract syntax tree (AST)
		ast = build_ast(tokens);
		if (!ast)
		{
			free_tokens(tokens);
			free(input);
			continue ;
		}

		// test
		// print_tokens(tokens);
		// print_ast(ast, 0);

		// Execute the command(s) represented by the AST
		exit_status = execute_command(ast, env, mini);

		if (exit_status == 5)
		{
			free_tokens(tokens);
			free_ast(ast);
			free(input);
			printf("exit\n");
			break ;
		}

		// Free allocated resources
		free_tokens(tokens);
		free_ast(ast);
		free(input);
	}
	// Free the environment
	free_env(mini.env);
	return (exit_status);
}
