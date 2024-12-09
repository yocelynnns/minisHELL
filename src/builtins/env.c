/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 06:51:58 by messs             #+#    #+#             */
/*   Updated: 2024/12/08 14:25:35 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_env(t_env *env)
{
	if (!env)
	{
		ft_putendl_fd("No environment variables found.", 1);
		return (SUCCESS);
	}
	while (env && env->next != NULL)
	{
		if (env->value)
			ft_putendl_fd(env->value, 1);
		env = env->next;
	}
	if (env && env->value)
		ft_putendl_fd(env->value, 1);
	return (SUCCESS);
}
