/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:47:21 by messs             #+#    #+#             */
/*   Updated: 2024/12/08 14:25:41 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	env_init(t_minishell *mini, char **env)
{
	t_env	*current;
	t_env	*new;
	int		i;

	if (!env || !env[0])
		return (0);
	current = malloc(sizeof(t_env));
	if (!env)
		return (0);
	current->value = ft_strdup(env[0]);
	current->next = NULL;
	mini->env = current;
	i = 1;
	while (env[i])
	{
		new = malloc(sizeof(t_env));
		if (!new)
		{
			free_env(mini->env);
			return (0);
		}
		new->value = ft_strdup(env[i]);
		new->next = NULL;
		current->next = new;
		current = new;
		i++;
	}
	return (1);
}
