/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:33:34 by messs             #+#    #+#             */
/*   Updated: 2024/12/20 17:08:41 by hthant           ###   ########.fr       */
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

int	is_valid_env(char *arg)
{
	int	i;

	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
int	add_or_update_env(char *arg, t_env *env)
{
	t_env	*tmp;
	char	*key;
	size_t	key_len;
	t_env	*new_node;
	char	*new_value;

	key_len = ft_strchr(arg, '=') ? (ft_strchr(arg, '=') - arg) : ft_strlen(arg);
	key = ft_substr(arg, 0, key_len);
	if (!key)
		return (print_export_error(-1, arg));
	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->value, key, key_len) == 0
			&& (tmp->value[key_len] == '=' || tmp->value[key_len] == '\0'))
		{
			free(tmp->value);
			new_value = ft_strdup(arg);
			if (!new_value)
			{
				free(key);
				return (print_export_error(-1, arg));
			}
			tmp->value = new_value;
			free(key);
			return (SUCCESS);
		}
		tmp = tmp->next;
	}
	free(key);
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (print_export_error(-1, arg));
	new_value = ft_strdup(arg);
	if (!new_value)
	{
		free(new_node);
		return (print_export_error(-1, arg));
	}
	new_node->value = new_value;
	new_node->next = NULL;
	if (!env)
		env = new_node;
	else
	{
		tmp = env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
	return (SUCCESS);
}

int	ft_export(char **args, t_env *env)
{
	int	i;
	int	error;

	if (!args[1])
	{
		print_sorted_env(env);
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
