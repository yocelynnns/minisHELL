/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 00:13:02 by yocelynnns        #+#    #+#             */
/*   Updated: 2025/02/20 17:15:09 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	prompt(char *input, t_minishell *mini)
{
	handle_eof(input, mini);
	if (*input)
		add_history(input);
}

char	*handle_input(t_minishell *mini)
{
	char	*input;

	init_signals();
	input = readline("minishell$ ");
	if (g_sigint == 1)
	{
		g_sigint = 0;
		mini->exit = 130;
	}
	stop_signals();
	prompt(input, mini);
	return (input);
}

t_token	*process_tokens(char *input, t_minishell *mini)
{
	t_token	*tokens;

	tokens = lexer(input, mini);
	mini->token = tokens;
	if (!tokens)
	{
		free_tokens(tokens);
		free(input);
		return (NULL);
	}
	return (tokens);
}

t_ast_node	*process_ast(t_token *tokens, char *input, t_minishell *mini)
{
	t_ast_node	*ast;

	ast = build_ast(tokens, mini);
	mini->ast = ast;
	if (!ast)
	{
		free_ast(ast);
		free_tokent(tokens, input);
		return (NULL);
	}
	return (ast);
}

void	run_shell_loop(t_minishell *mini)
{
	char		*input;
	t_token		*tokens;
	t_ast_node	*ast;

	while (1)
	{
		input = handle_input(mini);
		tokens = process_tokens(input, mini);
		if (!tokens)
			continue ;
		ast = process_ast(tokens, input, mini);
		if (!ast)
			continue ;
		mini->exit = 0;
		execute_command(mini->ast, mini);
		free_minishell(mini, input);
	}
}
