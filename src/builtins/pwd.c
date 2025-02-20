/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 06:57:56 by messs             #+#    #+#             */
/*   Updated: 2025/02/20 19:37:38 by hthant           ###   ########.fr       */
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

int	handle_tilde(char **path, t_env *env_list, t_minishell *mini)
{
	char	*home;
	char	*expanded_path;

	if (!path || !(*path) || (*path)[0] != '~')
		return (SUCCESS);
	home = get_env_variable(env_list, "HOME=", 5);
	if (!home)
	{
		ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
		mini->exit = 1;
		return (ERROR);
	}
	expanded_path = ft_strjoin(home, (*path) + 1);
	if (!expanded_path)
	{
		free(home);
		return (ERROR);
	}
	free(*path);
	*path = expanded_path;
	free(home);
	return (SUCCESS);
}

int	handle_special_cd(char **arguments, t_env *env_list, t_minishell *mini)
{
	char	*directory_path;
	int		result;

	if (ft_strcmp(arguments[1], "-") == 0)
	{
		directory_path = get_env_variable(env_list, "OLDPWD=", 7);
		printf("%s\n", directory_path);
		if (!directory_path)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR_FILENO);
			return (ERROR);
		}
		result = chdir(directory_path);
		if (result != 0)
			print_cd_error(directory_path, mini);
		free(directory_path);
	}
	return (SUCCESS);
}

int	handle_regular_cd(char *path, t_env *env_list, t_minishell *mini)
{
	char	*expanded_path;
	int		cd_result;

	expanded_path = ft_strdup(path);
	if (!expanded_path)
		return (ERROR);
	if (handle_tilde(&expanded_path, env_list, mini) != SUCCESS)
	{
		free(expanded_path);
		return (ERROR);
	}
	if (update_oldpwd(env_list) != SUCCESS)
	{
		free(expanded_path);
		return (ERROR);
	}
	cd_result = chdir(expanded_path);
	if (cd_result != 0)
		print_cd_error(expanded_path, mini);
	free(expanded_path);
	return (cd_result);
}
