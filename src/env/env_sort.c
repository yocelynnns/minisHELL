/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 19:24:23 by hthant            #+#    #+#             */
/*   Updated: 2024/12/11 19:30:13 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	count_env_vars(t_env *env)
{
	int	count;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

char	**env_to_array(t_env *env, int count)
{
	t_env	*tmp;
	char	**env_array;
	int		i;

	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		env_array[i] = tmp->value;
		i++;
		tmp = tmp->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

void	sort_env_array(char **env_array, int count)
{
	char	*temp;
	int		i;
	int		j;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(env_array[i], env_array[j]) > 0)
			{
				temp = env_array[i];
				env_array[i] = env_array[j];
				env_array[j] = temp;
			}
			j++;
		}
		i++;
	}
}

void	print_sorted_env(t_env *env)
{
	char	**env_array;
	int		count;
	int		i;

	count = count_env_vars(env);
	env_array = env_to_array(env, count);
	if (!env_array)
		return ;

	sort_env_array(env_array, count);

	i = 0;
	while (i < count)
	{
		ft_putendl_fd(env_array[i], STDOUT);
		i++;
	}

	free(env_array);
}
