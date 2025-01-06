/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:51:00 by ysetiawa          #+#    #+#             */
/*   Updated: 2025/01/06 16:45:35 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int g_exit_status = 0;

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_token		*tokens;
	t_ast_node	*ast;
	t_minishell	mini;

	(void)ac;
	(void)av;

	// Initialize environment variables
	if (env_init(&mini, env) == ERROR)
	{
		fprintf(stderr, "Error: Failed to initialize environment variables\n");
		return (EXIT_FAILURE);
	}
	{
		printf(" __   __  ___   __    _  ___   _______  __   __  _______  ___      ___     \n|  |_|  ||   | |  |  | ||   | |       ||  | |  ||       ||   |    |   |    \n|       ||   | |   |_| ||   | |  _____||  |_|  ||    ___||   |    |   |    \n|       ||   | |       ||   | | |_____ |       ||   |___ |   |    |   |    \n|       ||   | |  _    ||   | |_____  ||       ||    ___||   |___ |   |___ \n| ||_|| ||   | | | |   ||   |  _____| ||   _   ||   |___ |       ||       |\n|_|   |_||___| |_|  |__||___| |_______||__| |__||_______||_______||_______|\n");
	}
	init_signals();
	// print_sorted_env(mini.env);
	while (1)
	{
		signal(SIGINT, sig_int_handler);
		signal(SIGQUIT, SIG_IGN);

		input = readline("minishell$ ");
		handle_eof(input);

		if (*input)
			add_history(input);

		tokens = lexer(input);
		print_tokens(tokens);
		if (!tokens)
		{
			free(input);
			continue;
		}

		ast = build_ast(tokens);
		// print_ast(ast, 0);
		if (!ast)
		{
			free_tokens(tokens);
			free(input);
			continue;
		}
		execute_command(ast, env, mini);

		free_tokens(tokens);
		free_ast(ast);
		free(input);
	}
	// // Free the environment
	free_env(mini.env);
	return (g_exit_status);
}
