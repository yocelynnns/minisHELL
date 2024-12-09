/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messs <messs@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 18:05:42 by messs             #+#    #+#             */
/*   Updated: 2024/12/09 18:29:16 by messs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (unsigned char)(*s1) - (unsigned char)(*s2);
}

int	number_of_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

char *remove_quotes(const char *str)
{
    char *result;
    size_t len, i, j;

    if (!str)
        return NULL;

    len = ft_strlen(str);
    if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
    {
        result = (char *)malloc(len - 2);
        if (!result)
            return NULL;

        i = 1; 
        j = 0;
        while (i < len - 1)
        {
            result[j] = str[i];
            i++;
            j++;
        }
        result[j] = '\0'; 
    }
    else
        result = ft_strdup(str);

    return result;
}

int ft_echo(char **args)
{
    int i;
    int n_contains;
    char *arg_no_quotes;

    i = 1;
    n_contains = 0;

    while (args[i] && ft_strcmp(args[i], "-n") == 0)
    {
        n_contains = 1;
        i++;
    }
    while (args[i])
    {
        arg_no_quotes = remove_quotes(args[i]);
        if (arg_no_quotes)
        {
            ft_putstr_fd(arg_no_quotes, 1);
            free(arg_no_quotes);
        }
        if (args[i + 1] && args[i][0] != '\0')
            write(1, " ", 1);
        i++;
    }
    if (n_contains == 0)
        write(1, "\n", 1);

    return (SUCCESS);
}




// need to check for test case such as ( echo "hello world" )
// bash output is:
// bash$ echo "hello world"
// hello world
// when i test:
// minishell$ echo "hello world"
// "hello world"
// we cannot have the double quote