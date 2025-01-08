/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sec_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 17:45:45 by hthant            #+#    #+#             */
/*   Updated: 2025/01/08 17:50:18 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_exit_error(char **av)
{
	ft_putstr_fd("minishell: exit: ", STDERR);
	ft_putstr_fd(av[1], STDERR);
	ft_putendl_fd(": numeric argument required", STDERR);
}
