/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 01:44:24 by messs             #+#    #+#             */
/*   Updated: 2025/02/20 18:20:53 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_cd_error(const char *path, t_minishell *mini)
{
	if (access(path, 0) != 0)
		ft_putstr_fd("cd: no such file or directory: ", STDERR);
	else if (access(path, 1) != 0)
		ft_putstr_fd("cd: permission denied: ", STDERR);
	else
		ft_putstr_fd("cd: error: ", STDERR);
	ft_putstr_fd((char *)path, STDERR);
	ft_putendl_fd("", STDERR);
	mini->exit = 1;
}

int	update_oldpwd(t_env *env_list)
{
	char	current_directory[PATH_MAX];
	char	*new_value;
	t_env	*oldpwd_node;

	if (!getcwd(current_directory, sizeof(current_directory)))
		return (ERROR);
	oldpwd_node = env_list;
	while (oldpwd_node)
	{
		if (ft_strncmp(oldpwd_node->value, "OLDPWD=", 7) == 0)
		{
			free(oldpwd_node->value);
			new_value = malloc(ft_strlen("OLDPWD=") + \
			ft_strlen(current_directory) + 1);
			if (!new_value)
				return (ERROR);
			ft_memcpy(new_value, "OLDPWD=", 7);
			ft_memcpy(new_value + 7, current_directory,
				ft_strlen(current_directory) + 1);
			oldpwd_node->value = new_value;
			return (SUCCESS);
		}
		oldpwd_node = oldpwd_node->next;
	}
	return (SUCCESS);
}

int	handle_normal_cd(t_env *env_list, t_minishell *mini)
{
	char	*home;
	int		result;

	home = get_env_variable(env_list, "HOME=", 5);
	if (!home)
	{
		ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
		mini->exit = 1;
		return (ERROR);
	}
	result = chdir(home);
	free(home);
	if (result != 0)
	{
		print_cd_error(home, mini);
		return (ERROR);
	}
	return (SUCCESS);
}

int	ft_cd(char **arguments, t_env *env_list, t_minishell *mini)
{
	if (arguments[1] && arguments[2])
	{
		ft_putstr_fd("cd: too many arguments", STDERR);
		mini->exit = 1;
	}
	if (!arguments[1] || ft_strcmp(arguments[1], "") == 0)
		return (handle_normal_cd(env_list, mini));
	if (arguments[1] && ft_strcmp(arguments[1], "-") == 0)
		return (handle_special_cd(arguments, env_list, mini));
	return (handle_regular_cd(arguments[1], env_list, mini));
}
