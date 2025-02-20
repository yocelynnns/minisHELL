/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:09:48 by ysetiawa          #+#    #+#             */
/*   Updated: 2025/02/20 19:38:40 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	put_env_i(char **env_array)
{
	char	cwd[PATH_MAX];

	getcwd(cwd, PATH_MAX);
	env_array[0] = ft_strjoin("PWD=", cwd);
	env_array[1] = ft_strdup("SHLVL=1");
	env_array[2] = ft_strdup("_=/usr/bin/env");
	env_array[3] = NULL;
}

int	env_init(t_minishell *mini, char **env_array)
{
	if (!env_array || !env_array[0])
		put_env_i(env_array);
	if (init_env_list(mini, env_array) != 0)
		return (1);
	mini->env2 = env_list_to_array(mini->env);
	if (!mini->env2)
	{
		ft_putstr_fd("Error: Failed to convert env_var to array\n", STDERR);
		return (1);
	}
	return (0);
}

int	count_env_vars(t_env *env)
{
	int		count;
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

int	is_valid_env(char *arg)
{
	int	i;

	if (!arg || !ft_isalpha(arg[0]))
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
