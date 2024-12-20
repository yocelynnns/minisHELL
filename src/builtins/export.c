/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 19:44:08 by hthant            #+#    #+#             */
/*   Updated: 2024/12/20 20:15:48 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	is_valid_env(const char *arg)
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

int	env_add(const char *value, t_env **env)
{
	t_env	*new;
	t_env	*tmp;

	if (!env || !*env)
	{
		*env = malloc(sizeof(t_env));
		if (!*env)
			return (ERROR);
		(*env)->value = ft_strdup(value);
		(*env)->next = NULL;
		return (SUCCESS);
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return (ERROR);
	new->value = ft_strdup(value);
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->next = NULL;
	return (SUCCESS);
}

char	*get_env_name(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] && src[i] != '=')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	is_in_env(t_env *env, const char *arg)
{
	char	var_name[BUFF_SIZE];
	char	env_name[BUFF_SIZE];

	get_env_name(var_name, arg);
	while (env)
	{
		get_env_name(env_name, env->value);
		if (ft_strcmp(var_name, env_name) == 0)
		{
			free(env->value);
			env->value = ft_strdup(arg);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

int	ft_export(char **args, t_env *env, t_env *copy_env)
{
	int		error_ret;
	char	*temp;

	if (!args[1])
	{
		print_sorted_env(copy_env);
		return (SUCCESS);
	}
	error_ret = is_valid_env(args[1]);
	if (args[1][0] == '=')
		error_ret = -3;
	if (error_ret <= 0)
		return (print_export_error(error_ret, args[1]));
	if (!ft_strchr(args[1], '='))
	{
		temp = ft_strjoin(args[1], "=");
		if (!is_in_env(env, temp))
		{
			if (env_add(temp, &env) == ERROR)
				return (print_export_error(-1, args[1]));
		}
		if (!is_in_env(copy_env, temp))
		{
			if (env_add(temp, &copy_env) == ERROR)
				return (print_export_error(-1, args[1]));
		}
		free(temp);
	}
	else
	{
		if (!is_in_env(env, args[1]))
		{
			if (env_add(args[1], &env) == ERROR)
				return (print_export_error(-1, args[1]));
		}
		if (!is_in_env(copy_env, args[1]))
		{
			if (env_add(args[1], &copy_env) == ERROR)
				return (print_export_error(-1, args[1]));
		}
	}
	return (SUCCESS);
}
