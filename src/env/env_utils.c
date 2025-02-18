/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:59:06 by ysetiawa          #+#    #+#             */
/*   Updated: 2025/02/18 15:09:22 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	count_env_nodes(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

char	**allocate_env_array(int count)
{
	char	**env_array;

	env_array = malloc((count + 1) * sizeof(char *));
	if (!env_array)
		return (NULL);
	return (env_array);
}

int	fill_env_array(t_env *env_list, char **env_array, int count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < count)
	{
		env_array[i] = strdup(env_list->value);
		if (!env_array[i])
		{
			while (j < i)
			{
				free(env_array[j]);
				j++;
			}
			free(env_array);
			return (0);
		}
		env_list = env_list->next;
		i++;
	}
	env_array[count] = NULL;
	return (1);
}

char	**env_list_to_array(t_env *env_list)
{
	char	**env_array;
	int		count;

	count = count_env_nodes(env_list);
	env_array = allocate_env_array(count);
	if (!env_array)
		return (NULL);
	if (!fill_env_array(env_list, env_array, count))
		return (NULL);
	return (env_array);
}

// char	**env_list_to_array(t_env *env_list)
// {
// 	t_env	*current;
// 	int		count;
// 	char	**env_array;
// 	int		i;
// 	int		j;

// 	current = env_list;
// 	count = 0;
// 	while (current)
// 	{
// 		count++;
// 		current = current->next;
// 	}
// 	env_array = malloc((count + 1) * sizeof(char *));
// 	if (!env_array)
// 		return (NULL);
// 	current = env_list;
// 	i = 0;
// 	while (i < count)
// 	{
// 		env_array[i] = strdup(current->value);
// 		if (!env_array[i])
// 		{
// 			j = 0;
// 			while (j < i)
// 			{
// 				free(env_array[j]);
// 				j++;
// 			}
// 			free(env_array);
// 			return (NULL);
// 		}
// 		current = current->next;
// 		i++;
// 	}
// 	env_array[count] = NULL;
// 	return (env_array);
// }