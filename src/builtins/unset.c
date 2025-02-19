/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 12:23:36 by hthant            #+#    #+#             */
/*   Updated: 2025/02/19 15:03:02 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

size_t	env_len(char *env)
{
	size_t	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	return (i);
}

int	unset_env_var(t_minishell *mini, char *arg)
{
	t_env	*env;
	t_env	*prev;

	if (!arg || !mini->env)
		return (SUCCESS);
	env = mini->env;
	prev = NULL;
	while (env)
	{
		if (ft_strncmp(arg, env->value, env_len(env->value)) == 0
			&& env_len(env->value) == ft_strlen(arg))
		{
			if (prev == NULL)
				mini->env = env->next;
			else
				prev->next = env->next;
			return (free_node(mini, env), SUCCESS);
		}
		prev = env;
		env = env->next;
	}
	return (SUCCESS);
}


int	ft_unset(char **args, t_minishell *mini)
{
	int	i;

	if (!args[1])
		return (SUCCESS);
	i = 1;
	while (args[i])
	{
		unset_env_var(mini, args[i]);
		i++;
	}
	return (0);
}
