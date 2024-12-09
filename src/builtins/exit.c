/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messs <messs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 16:54:12 by messs             #+#    #+#             */
/*   Updated: 2024/12/10 02:06:57 by messs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	check_is_number(char *str)
{
	int i;

	if (str == NULL || *str == '\0')
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0); 
		i++;
	}
	return (1);
}

void	ft_exit(t_minishell *mini, char **av)
{
    mini->exit = 1;
	ft_putstr_fd("exit\n", STDERR);

	if (av[1] && av[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR);
		exit(1); 
	}
	if (av[1] && check_is_number(av[1]) == 0)
	{
		ft_putstr_fd("minishell: exit: ", STDERR);
		ft_putstr_fd(av[1], STDERR);
		ft_putendl_fd(": numeric argument required", STDERR);
		exit(255); 
	}

	if (av[1])
		exit(ft_atoi(av[1]) % 256);

	exit(0); 
}
// ft_atoi(av[1]). Then, it takes the result modulo 256 (with % 256) 
// to ensure that the exit status is within the valid range of 0–255. 
// This is done because exit statuses in Unix-based systems are limited to the range 0–255, 
// and Bash uses this behavior when exiting with a numeric value