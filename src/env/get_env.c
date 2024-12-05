/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:47:21 by messs             #+#    #+#             */
/*   Updated: 2024/12/05 17:46:27 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// int env_init(t_minishell *mini, char **env)
// {
//     t_env *current;
//     t_env *new;
//     int i;

//     if(!env || !env[0])
//         return 0;
//     if(!env = malloc(sizeof(t_env)))
//         return 0;
//     current->value = ft_strdup(env[0]);
//     current->next = NULL;
//     mini->env = current;

//     i = 1;
//     while (env[i])
//     {
//         new = malloc(sizeof(t_env));
//         if(!new)
//         {
//             free_env(mini->env);
//             return 0;
//         }
//         new->value = ft_strdup(env[i]);
//         new->next = NULL;
//         current->next = new;
//         current = new;
//         i++;
//     }
//     return 1;
    
// }