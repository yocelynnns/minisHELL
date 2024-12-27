/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 06:51:58 by messs             #+#    #+#             */
/*   Updated: 2024/12/27 15:37:38 by hthant           ###   ########.fr       */
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
	while (env)
	{
		if (env->value)
			ft_putendl_fd(env->value, 1);
		env = env->next;
	}
	return (SUCCESS);
}

int	update_env(char *key, char *new_value, t_env **env)
{
	t_env	*tmp;
	size_t	key_len;

	tmp = *env;
	key_len = ft_strlen(key);
	while (tmp)
	{
		if (ft_strncmp(tmp->value, key, key_len) == 0
			&& tmp->value[key_len] == '=')
		{
			free(tmp->value);
			tmp->value = new_value;
			if (!tmp->value)
				return (print_export_error(-1, key));
			return (SUCCESS);
		}
		tmp = tmp->next;
	}
	return (ERROR);
}

int	add_env(char *new_value, t_env **env)
{
	t_env	*new_node;
	t_env	*last;
	char	*value_copy;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		value_copy = strdup(new_value);
		return (print_export_error(-1, value_copy));
	}
	new_node->value = new_value;
	new_node->next = NULL;
	if (!*env)
		*env = new_node;
	else
	{
		last = *env;
		while (last->next)
			last = last->next;
		last->next = new_node;
	}
	return (SUCCESS);
}

char	*get_env_variable(t_env *env_list, const char *variable, size_t len)
{
	while (env_list)
	{
		if (env_list->value && ft_strncmp(env_list->value, variable, len) == 0)
			return (ft_strdup(env_list->value + len));
		env_list = env_list->next;
	}
	return (NULL);
}

char	*get_env_value(const char *key, t_env *env)
{
	char	*delimiter;
	size_t	key_len;

	if (!key || !env)
		return (NULL);
	while (env)
	{
		delimiter = ft_strchr(env->value, '=');
		if (delimiter)
		{
			key_len = delimiter - env->value;
			if (ft_strlen(key) == key_len && ft_strncmp(env->value, key,
					key_len) == 0)
				return (delimiter + 1);
		}
		env = env->next;
	}
	return (NULL);
}
