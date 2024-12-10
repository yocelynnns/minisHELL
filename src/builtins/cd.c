/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 01:44:24 by messs             #+#    #+#             */
/*   Updated: 2024/12/10 15:39:27 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void print_cd_error(const char *path)
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

char *find_env_variable(t_env *env_list, const char *variable, size_t len)
{
    while (env_list)
    {
        if (ft_strncmp(env_list->value, variable, len) == 0)
            return ft_strdup(env_list->value + len);
        env_list = env_list->next;
    }
    return NULL;
}

int update_previous_directory(t_env *env_list)
{
    char current_directory[PATH_MAX];
    char *previous_directory;

    if (!getcwd(current_directory, PATH_MAX))
        return ERROR;
    previous_directory = malloc(ft_strlen("OLDPWD=") + ft_strlen(current_directory) + 1);
    if (!previous_directory)
        return ERROR;
    ft_memcpy(previous_directory, "OLDPWD=", 7);
    ft_memcpy(previous_directory + 7, current_directory, ft_strlen(current_directory) + 1);

    free_env(env_list);
    env_init((t_minishell *)&env_list, &previous_directory);
    free(previous_directory);
    return SUCCESS;
}

int navigate_to_special_directory(int option, t_env *env_list)
{
    char *directory_path;
    int ret;

    directory_path = NULL;
    if (option == 0)
    {
        directory_path = find_env_variable(env_list, "HOME=", 5);
        if (!directory_path)
        {
            ft_putendl_fd("minishell: cd: HOME not set", STDERR);
            return ERROR;
        }
    }
    else if (option == 1)
    {
        directory_path = find_env_variable(env_list, "OLDPWD=", 7);
        if (!directory_path)
        {
            ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR);
            return ERROR;
        }
        ft_putendl_fd(directory_path, STDOUT_FILENO);
    }
    if (update_previous_directory(env_list) != SUCCESS)
    {
        free(directory_path);
        return ERROR;
    }
    ret = chdir(directory_path);
    free(directory_path);
    return ret;
}

int ft_cd(char **arguments, t_env *env_list)
{
    int cd_result;

    if (!arguments[1])
        return navigate_to_special_directory(0, env_list);
    if (ft_strcmp(arguments[1], "-") == 0)
        cd_result = navigate_to_special_directory(1, env_list);
    else
    {
        if (update_previous_directory(env_list) != SUCCESS)
            return ERROR;
        cd_result = chdir(arguments[1]);
        if (cd_result != 0)
            print_cd_error(arguments[1]);
    }
    if (cd_result == 0)
        return SUCCESS;
    return ERROR;
}
