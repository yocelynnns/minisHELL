/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 06:55:13 by messs             #+#    #+#             */
/*   Updated: 2024/12/08 14:25:30 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (unsigned char)(*s1) - (unsigned char)(*s2);
}

int	number_of_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	ft_echo(char **args)
{
	int	i;
	int	n_contains;

	i = 1;
	n_contains = 0;
	if (number_of_args(args) > 1)
	{
		while (args[i] && ft_strcmp(args[i], "-n") == 0)
		{
			n_contains = 1;
			i++;
		}
		while (args[i])
		{
			ft_putstr_fd(args[i], 1);
			if (args[i + 1])
				write(1, " ", 1);
			i++;
		}
	}
	if (n_contains == 0)
		write(1, "\n", 1);
	return (SUCCESS);
}

// need to check for test case such as ( echo "hello world" )
// bash output is:
// bash$ echo "hello world"
// hello world
// when i test:
// minishell$ echo "hello world"
// "hello world"
// we cannot have the double quote
