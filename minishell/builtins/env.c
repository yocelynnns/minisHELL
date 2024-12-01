/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messs <messs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 06:51:58 by messs             #+#    #+#             */
/*   Updated: 2024/12/02 07:37:24 by messs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_env(t_env *env)
{
    if (!env) {
        ft_putendl_fd("No environment variables found.", 1);
        return (SUCCESS);
    }
    while (env && env->next != NULL)
    {
        if (env->value)  // Check if value is non-null
            ft_putendl_fd(env->value, 1);
        env = env->next;
    }

    if (env && env->value)  // Print the last element if exists
        ft_putendl_fd(env->value, 1);

    return (SUCCESS);
}
