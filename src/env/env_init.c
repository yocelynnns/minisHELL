/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:53:38 by hthant            #+#    #+#             */
/*   Updated: 2024/12/12 11:56:10 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	env_init(t_minishell *mini, char **env_array)
{
	t_env	*env;
	t_env	*new;
	int		i;

	if (!env_array || !env_array[0])
	{
		ft_putendl_fd("env_array is empty or NULL.", 2);
		return (1);
	}
	env = malloc(sizeof(t_env));
	if (!env)
	{
		ft_putendl_fd("Malloc failed for env head.", 2);
		return (1);
	}
	env->value = ft_strdup(env_array[0]);
	if (!env->value)
	{
		ft_putendl_fd("ft_strdup failed for first env value.", 2);
		free(env);
		return (1);
	}
	env->next = NULL;
	mini->env = env;
	i = 1;
	while (env_array[i])
	{
		new = malloc(sizeof(t_env));
		if (!new)
		{
			ft_putendl_fd("Malloc failed for new env node.", 2);
			free_env(mini->env);
			return (1);
		}
		new->value = ft_strdup(env_array[i]);
		if (!new->value)
		{
			ft_putendl_fd("ft_strdup failed for env value.", 2);
			free(new);
			free_env(mini->env);
			return (1);
		}
		new->next = NULL;
		env->next = new;
		env = new;
		i++;
	}
	ft_putendl_fd("Environment variables successfully initialized.", 2);
	return (0);
}

