/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:10:03 by hthant            #+#    #+#             */
/*   Updated: 2025/02/17 15:11:40 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ignore_sigquit(int signo)
{
	(void)signo;
	ft_putstr_fd("\033[2K\r", STDERR_FILENO);
	ft_putstr_fd("> ", STDERR);
}

void	init_signals(void)
{
	set_signals_interactive();
}

void	handle_eof(char *line, t_minishell *mini)
{
	int	i;

	if (!line)
	{
		i = 0;
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		free_env(mini->env);
		free_env_array(mini->env2);
		free(mini);
		exit(i);
	}
}

void	stop_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
