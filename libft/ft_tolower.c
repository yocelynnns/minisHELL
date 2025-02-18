/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:23:57 by hthant            #+#    #+#             */
/*   Updated: 2025/02/18 22:44:46 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_tolower(int c);

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}

// int main(void)
// {
// 	int i = tolower(99);
// 	int j = tolower(65);
// 	int k = tolower(55);
// 	printf("%c\n%c\n%c\n",i,j,k);
// }
