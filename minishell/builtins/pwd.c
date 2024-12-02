/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messs <messs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 06:57:56 by messs             #+#    #+#             */
/*   Updated: 2024/12/02 12:23:47 by messs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_pwd(void)
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, PATH_MAX))
    {
        ft_putendl_fd(cwd, 1);
        return SUCCESS;
    }
    else
    {
        ft_putstr_fd("minishell: pwd: ", STDERR);
        perror("");
        return ERROR;
    }
}
