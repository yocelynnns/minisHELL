/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messs <messs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 06:55:13 by messs             #+#    #+#             */
/*   Updated: 2024/12/02 07:32:30 by messs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}

int number_of_args(char **args)
{
    int i;

    i = 0;
    while (args[i])
        i++;
    return i;
}

int ft_echo(char **args)
{
    int i;
    int n_contains;

    i = 1;
    n_contains = 0;

    // Check if more than one argument is passed
    if (number_of_args(args) > 1)
    {
        // Check for the -n option (no newline)
        while (args[i] && ft_strcmp(args[i], "-n") == 0)
        {
            n_contains = 1;
            i++;
        }

        // Print all arguments except the -n option
        while (args[i])
        {
            ft_putstr_fd(args[i], 1);
            if (args[i + 1])  // Avoid printing a space after the last argument
                write(1, " ", 1);
            i++;
        }
    }

    // Print newline unless -n option is specified
    if (n_contains == 0)
        write(1, "\n", 1);

    return SUCCESS;
}
