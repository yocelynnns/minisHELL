/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 06:57:56 by messs             #+#    #+#             */
/*   Updated: 2024/12/26 19:03:25 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
	{
		ft_putendl_fd(cwd, 1);
		return (SUCCESS);
	}
	else
	{
		ft_putstr_fd("minishell: pwd: ", STDERR);
		perror("");
		return (ERROR);
	}
}

int	handle_tilde(char **path, t_env *env_list)
{
	char	*home;
	char	*expanded_path;

	if (!path || !(*path) || (*path)[0] != '~')
		return (SUCCESS);
	home = get_env_variable(env_list, "HOME=", 5);
	if (!home)
	{
		ft_putendl_fd("minishell: cd: HOME not set", STDERR);
		return (ERROR);
	}
	expanded_path = ft_strjoin(home, (*path) + 1);
	free(home);
	if (!expanded_path)
		return (ERROR);
	free(*path);
	*path = expanded_path;
	return (SUCCESS);
}

int	handle_special_cd(char **arguments, t_env *env_list)
{
	int	cd_result;

	if (!arguments[1] || ft_strcmp(arguments[1], "~") == 0)
		return (navigate_to_special_dir(0, env_list));
	if (ft_strcmp(arguments[1], "-") == 0)
	{
		cd_result = navigate_to_special_dir(1, env_list);
		return (cd_result);
	}
	return (SUCCESS);
}

int	handle_regular_cd(char *path, t_env *env_list)
{
	int	cd_result;

	if (handle_tilde(&path, env_list) != SUCCESS)
		return (ERROR);
	if (update_oldpwd(env_list) != SUCCESS)
		return (ERROR);
	cd_result = chdir(path);
	if (cd_result != 0)
		print_cd_error(path);
	return (cd_result);
}
