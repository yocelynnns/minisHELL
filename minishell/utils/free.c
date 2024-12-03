/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messs <messs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:41:11 by messs             #+#    #+#             */
/*   Updated: 2024/12/03 15:04:05 by messs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

#include <stdlib.h>

void free_env(t_env *env) {
    t_env *tmp;

    while (env) {
        tmp = env->next;    // Save the next node
        free(env->value);   // Free the key-value string
        free(env);          // Free the current node
        env = tmp;          // Move to the next node
    }
}
