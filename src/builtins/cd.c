/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 01:44:24 by messs             #+#    #+#             */
/*   Updated: 2024/12/26 18:56:48 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_cd_error(const char *path)
{
	ft_putstr_fd("cd: ", STDERR);
	if (errno == ENOENT)
		ft_putstr_fd("no such file or directory: ", STDERR);
	else if (errno == EACCES)
		ft_putstr_fd("permission denied: ", STDERR);
	else
		ft_putstr_fd("error: ", STDERR);
	ft_putstr_fd((char *)path, STDERR);
	ft_putendl_fd("", STDERR);
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
			new_value = malloc(ft_strlen("OLDPWD=")
					+ ft_strlen(current_directory) + 1);
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

char	*get_special_directory_path(int option, t_env *env_list)
{
	char	*directory_path;

	directory_path = NULL;
	if (option == 0)
	{
		directory_path = get_env_variable(env_list, "HOME=", 5);
		if (!directory_path)
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
	}
	else if (option == 1)
	{
		directory_path = get_env_variable(env_list, "OLDPWD=", 7);
		if (!directory_path)
			ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR_FILENO);
		else
			ft_putendl_fd(directory_path, STDOUT_FILENO);
	}
	return (directory_path);
}

int	navigate_to_special_dir(int option, t_env *env_list)
{
	char	*directory_path;
	int		result;

	directory_path = get_special_directory_path(option, env_list);
	if (!directory_path)
		return (ERROR);
	if (update_oldpwd(env_list) != SUCCESS)
	{
		free(directory_path);
		return (ERROR);
	}
	result = chdir(directory_path);
	free(directory_path);
	return (result);
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

int	ft_cd(char **arguments, t_env *env_list)
{
	char	*path;
	int		cd_result;

	if (!arguments[1] || ft_strcmp(arguments[1], "~") == 0)
		return (navigate_to_special_dir(0, env_list));
	path = ft_strdup(arguments[1]);
	if (!path)
		return (ERROR);
	if (handle_tilde(&path, env_list) != SUCCESS)
	{
		free(path);
		return (ERROR);
	}
	if (ft_strcmp(path, "-") == 0)
		cd_result = navigate_to_special_dir(1, env_list);
	else
	{
		if (update_oldpwd(env_list) != SUCCESS)
		{
			free(path);
			return (ERROR);
		}
		cd_result = chdir(path);
		if (cd_result != 0)
			print_cd_error(path);
	}
	free(path);
	if (cd_result == 0)
		return (SUCCESS);
	return (ERROR);
}
