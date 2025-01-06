/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 16:54:12 by messs             #+#    #+#             */
/*   Updated: 2025/01/06 18:48:11 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

int	ft_exit(t_minishell *mini, char **av)
{
	long	exit_code;

	ft_putstr_fd("exit\n", STDERR);
	mini->exit = 1;
	if (av[1] && check_is_number(av[1]) == 0)
	{
		ft_putstr_fd("minishell: exit: ", STDERR);
		ft_putstr_fd(av[1], STDERR);
		ft_putendl_fd(": numeric argument required", STDERR);
		exit(2);
	}
	if (av[1] && av[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR);
		mini->exit = 0;
		return (1);
	}
	if (av[1])
	{
		exit_code = ft_atol(av[1]);
		exit(exit_code % 256);
	}
	exit(0);
	return(0);
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
