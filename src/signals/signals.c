/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 12:31:23 by messs             #+#    #+#             */
/*   Updated: 2024/12/18 15:10:52 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_signal g_sig;

void sig_int_handler(int code)
{
    (void)code;
    if (g_sig.pid == 0)
    {
        ft_putstr_fd("\n", STDERR);
        ft_putstr_fd("minishell$ ", STDERR);
        g_sig.exit_status = 1;
    }
    else
    {
        ft_putstr_fd("\n", STDERR);
        g_sig.exit_status = 130;
    }
    g_sig.sigint = 1;
}

void sig_quit_handler(int code)
{
    (void)code;
    if (g_sig.pid != 0)
    {
        ft_putstr_fd("Quit: 3\n", STDERR);
        g_sig.exit_status = 131;
        g_sig.sigquit = 1;
    }
    else
        ft_putstr_fd("\b\b  \b\b", STDERR);
}

void init_signals(void)
{
    g_sig.sigint = 0;
    g_sig.sigquit = 0;
    g_sig.pid = 0;
    g_sig.exit_status = 0;

    signal(SIGINT, sig_int_handler);
    signal(SIGQUIT, sig_quit_handler);
}

void handle_eof(char *line)
{
    if (!line)
    {
        ft_putstr_fd("exit\n", STDOUT);
        exit(g_sig.exit_status);
    }
}

