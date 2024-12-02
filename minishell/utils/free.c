/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messs <messs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:41:11 by messs             #+#    #+#             */
/*   Updated: 2024/12/02 14:46:56 by messs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*ft_memdel(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
	return (NULL);
}

void free_env(t_env *env)
{
    t_env *tmp;
    
    while (env && env->next)
    {
        tmp = env;
        env = env->next;
        ft_memdel(tmp->value);
        ft_memdel(tmp);
    }
    ft_memdel(env->value);
    ft_memdel(env);
    
}