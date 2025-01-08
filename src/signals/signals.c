/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 12:31:23 by messs             #+#    #+#             */
/*   Updated: 2025/01/08 19:27:16 by hthant           ###   ########.fr       */
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
	}
	g_exit_status = 130;
}

void	sig_quit_handler(int code)
{
	(void)code;
	if (g_sig.pid == 0)
	{
		rl_on_new_line();
		rl_redisplay();
	}
	else
	{
		ft_putstr_fd("Quit (core dumped)\n",STDERR);
		g_exit_status = 131;
	}
}

void	init_signals(void)
{
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	g_sig.pid = 0;
}

void	handle_eof(char *line)
{
	if (!line)
	{
		ft_putstr_fd("exit\n", STDOUT);
		exit(g_exit_status);
	}
}
