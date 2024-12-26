/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 19:44:08 by hthant            #+#    #+#             */
/*   Updated: 2024/12/26 18:36:12 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	print_export_error(int error, const char *arg)
{
	int	i;

	if (error == -1)
		ft_putstr_fd("export: not valid in this context: ", STDERR);
	else if (error == 0 || error == -3)
		ft_putstr_fd("export: not a valid identifier: ", STDERR);
	i = 0;
	while (arg[i] && (arg[i] != '=' || error == -3))
	{
		write(STDERR, &arg[i], 1);
		i++;
	}
	write(STDERR, "\n", 1);
	return (ERROR);
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

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(new_value);
		return (print_export_error(-1, new_value));
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

int	add_or_update_env(char *arg, t_env **env)
{
	char	*key;
	char	*new_value;
	size_t	key_len;

	if (ft_strchr(arg, '='))
		key_len = ft_strchr(arg, '=') - arg;
	else
		key_len = ft_strlen(arg);
	key = ft_substr(arg, 0, key_len);
	if (!key)
		return (print_export_error(-1, arg));
	if (ft_strchr(arg, '='))
		new_value = ft_strdup(arg);
	else
		new_value = ft_strjoin(key, "=");
	if (!new_value)
	{
		free(key);
		return (print_export_error(-1, arg));
	}
	if (update_env(key, new_value, env) == SUCCESS)
	{
		free(key);
		return (SUCCESS);
	}
	free(key);
	return (add_env(new_value, env));
}

int	ft_export(char **args, t_env **env)
{
	int	i;
	int	error;

	if (!args[1])
	{
		print_sorted_env(*env);
		return (SUCCESS);
	}
	i = 1;
	error = SUCCESS;
	while (args[i])
	{
		if (!is_valid_env(args[i]))
			error = print_export_error(0, args[i]);
		else if (add_or_update_env(args[i], env) == ERROR)
		{
			ft_putstr_fd("export: failed to allocate memory for: ", STDERR);
			ft_putendl_fd(args[i], STDERR);
			error = ERROR;
		}
		i++;
	}
	return (error);
}
