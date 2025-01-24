/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 16:54:12 by messs             #+#    #+#             */
/*   Updated: 2025/01/24 15:34:26 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <limits.h>

int	check_is_number(char *str)
{
	int	i;

	if (str == NULL || *str == '\0')
		return (0);
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	return (str[i] == '\0');
}

int	ft_exit(char **av, t_minishell *mini)
{
	long	exit_code;

	ft_putstr_fd("exit\n", STDERR);
	g_sig.exit_value = 0;
	if (av[1] && check_is_number(av[1]) == 0)
	{
		print_exit_error(av[1]); // Custom function to handle exit error printing
		cleanup(mini);        // Cleanup before exit
		exit(2);
	}
	if (av[1] && av[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR);
		g_sig.exit_value = 1; // Set exit value to 1 to indicate an error
		return (1);           // Return 1 instead of exiting immediately
	}
	if (av[1])
	{
		exit_code = ft_atol(av[1], mini);
		// if (exit_code > 9223372036854775807 || exit_code < -9223372036854775807 - 1)
		// {
		// 	print_exit_error(av); // Print an appropriate error message
		// 	cleanup(mini);        // Cleanup before exiting
		// 	exit(2);
		// }
		exit(exit_code % 256);
	}
	exit_code = g_sig.exit_value;
	cleanup(mini); // Cleanup before exiting
	exit(exit_code % 256);
}

int	add_env_node(char *new_value, t_env **env)
{
	t_env	*new_node;
	t_env	*last;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (print_export_error(-1, ft_strdup(new_value)));
	new_node->value = new_value;
	new_node->next = NULL;
	if (!*env)
		*env = new_node;
	else
	{
		last = *env;
		while (last->next)
			last = last->next;
		last->next = new_node;
	}
	return (SUCCESS);
}
