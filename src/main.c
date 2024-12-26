/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 17:51:00 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/26 14:14:53 by hthant           ###   ########.fr       */
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
	int			exit_status;

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
	// print_sorted_env(mini.env);
	while (1)
	{
		init_signals();
		signal(SIGINT, &sig_int_handler);
		signal(SIGQUIT, &sig_quit_handler);

		input = readline("minishell$ ");
		handle_eof(input);

		if (!input)
		{
			printf("exit\n");
			break;
		}

		if (*input)
			add_history(input);

		tokens = lexer(input);
		// print_tokens(tokens);
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

		exit_status = execute_command(ast, env, mini);
		g_exit_status = exit_status;

		if (exit_status == 5) // Exit command
		{
			free_tokens(tokens);
			free_ast(ast);
			free(input);
			printf("exit\n");
			break;
		}

		free_tokens(tokens);
		free_ast(ast);
		free(input);
	}
	// Free the environment
	free_env(mini.env);
	return (g_exit_status);
}
