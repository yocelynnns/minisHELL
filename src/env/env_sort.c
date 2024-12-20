/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messs <messs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 01:07:06 by messs             #+#    #+#             */
/*   Updated: 2024/12/21 01:09:07 by messs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int count_env_vars(t_env *env)
{
	int count = 0;
	t_env *tmp = env;

	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

char **env_to_array(t_env *env, int count)
{
	char **env_array;
	t_env *tmp;
	int i;

	if (!env || count <= 0)
		return (NULL);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		env_array[i] = ft_strdup(tmp->value);
		if (!env_array[i])
		{
			while (i > 0)
				free(env_array[--i]);
			free(env_array);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

void sort_env_array(char **env_array, int count)
{
	char *temp;
	int i, j;

	for (i = 0; i < count - 1; i++)
	{
		for (j = i + 1; j < count; j++)
		{
			if (ft_strcmp(env_array[i], env_array[j]) > 0)
			{
				temp = env_array[i];
				env_array[i] = env_array[j];
				env_array[j] = temp;
			}
		}
	}
}

void print_sorted_env(t_env *env)
{
	char **env_array;
	int count;
	int i;

	count = count_env_vars(env);
	if (count == 0)
		return;
	env_array = env_to_array(env, count);
	if (!env_array)
	{
		ft_putstr_fd("Error: Memory allocation failed in print_sorted_env\n", STDERR);
		return;
	}
	sort_env_array(env_array, count);
	i = 0;
	while (i < count)
	{
		ft_putendl_fd(env_array[i], STDOUT);
		free(env_array[i]);
		i++;
	}
	free(env_array);
}
