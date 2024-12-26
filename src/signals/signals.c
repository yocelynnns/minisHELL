/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 12:31:23 by messs             #+#    #+#             */
/*   Updated: 2024/12/26 15:09:35 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_signal	g_sig = {0};

void	sig_int_handler(int code)
{
	(void)code;
	if (g_sig.pid == 0)
	{
		ft_putstr_fd("\n", STDERR);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
	{
		ft_putstr_fd("\n", STDERR);
		g_sig.exit_status = 130;
	}
	g_sig.sigint = 1;
}

void	sig_quit_handler(int code)
{
	(void)code;
	if (g_sig.pid == 0)
	{
		write(STDERR, "\033[2D  \033[2D", 11);
		rl_on_new_line();
		rl_redisplay();
	}
	else
	{
		write(STDERR, "Quit: 3\n", 8);
		g_sig.exit_status = 131;
	}
}

void	init_signals(void)
{
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	g_sig.pid = 0;
	g_sig.exit_status = 0;
}

void	handle_eof(char *line)
{
	if (!line)
	{
		ft_putstr_fd("exit\n", STDOUT);
		exit(g_sig.exit_status);
	}
}
