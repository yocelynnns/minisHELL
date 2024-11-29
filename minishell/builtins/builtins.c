/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messs <messs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:32:04 by messs             #+#    #+#             */
/*   Updated: 2024/11/30 06:10:57 by messs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void print_error(const char *msg, const char *arg)
{
    write(2, "cd: ", 5);
    if (msg)
        write(2, msg, ft_strlen(msg));
    if (arg)
    {
        write(2, ": ", 2);
        write(2, arg, ft_strlen(arg));
    }
    write(2, "\n", 1);
}

char *get_env_path(t_env *env, const char *key)
{
    size_t key_len;
    
    key_len = ft_strlen(key);

    while (env)
    {
        if (ft_strncmp(env->value, key, key_len) == 0 && env->value[key_len] == '=')
            return env->value + key_len + 1;  // Return value part after the '='
        env = env->next;
    }
    return NULL;  // Return NULL if the key is not found
}

void env_add(const char *new_entry, t_env *env)
{
    t_env *current = env;
    size_t key_len;
    t_env *new_node;
    size_t i;

    key_len = 0;

    while (new_entry[key_len] != '=' && new_entry[key_len] != '\0')
        key_len++;

    while (current)
    {
        if (ft_strncmp(current->value, new_entry, key_len) == 0 && current->value[key_len] == '=')
        {
            // Free existing value and update it with the new entry
            free(current->value);
            current->value = malloc(ft_strlen(new_entry) + 1);
            if (current->value)
            {
                size_t i = 0;
                while (new_entry[i] != '\0')
                {
                    current->value[i] = new_entry[i];
                    i++;
                }
                current->value[i] = '\0';
            }
            return;
        }
        current = current->next;
    }

    // Add new entry if the key was not found
    new_node = malloc(sizeof(t_env));
    if (!new_node)
        return;
    new_node->value = malloc(ft_strlen(new_entry) + 1);
    if (new_node->value)
    {
        i = 0;
        while (new_entry[i] != '\0')
        {
            new_node->value[i] = new_entry[i];
            i++;
        }
        new_node->value[i] = '\0';
    }
    new_node->next = env->next;
    env->next = new_node;
}

int builtin_cd(char **args, t_env *env)
{
    char *target_dir = NULL;
    char cwd[PATH_MAX];
    char *oldpwd;
    char *pwd;

    if (!args[1])
        target_dir = get_env_path(env, "HOME");
    else if (strcmp(args[1], "-") == 0)
        target_dir = get_env_path(env, "OLDPWD");
    else
        target_dir = args[1];

    if (!target_dir)
    {
        if (args[1])
            print_error("OLDPWD not set", NULL);
        else
            print_error("HOME not set", NULL);
        return 1;
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        print_error("getcwd failed", NULL);
        return 1;
    }

    if (chdir(target_dir) != 0)
    {
        print_error("chdir failed", args[1]);
        return 1;
    }

    // Update environment variables without dynamic allocation
    oldpwd = get_env_path(env, "OLDPWD");
    if (oldpwd)
    {
        // Overwrite the existing value
        strncpy(oldpwd, "OLDPWD=", 8);
        strncat(oldpwd, cwd, PATH_MAX - 8);
    } else
    {
        // Create a new environment variable
        env_add("OLDPWD=", env);
        oldpwd = get_env_path(env, "OLDPWD");
        strncat(oldpwd, cwd, PATH_MAX - 8);
    }

    pwd = get_env_path(env, "PWD");
    if (pwd)
    {
        // Overwrite the existing value
        strncpy(pwd, "PWD=", 5);
        strncat(pwd, target_dir, PATH_MAX - 5);
    } else
    {
        // Create a new environment variable
        env_add("PWD=", env);
        pwd = get_env_path(env, "PWD");
        strncat(pwd, target_dir, PATH_MAX - 5);
    }
    return 0;
}
