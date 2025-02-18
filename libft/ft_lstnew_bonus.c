/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yocelynnns <yocelynnns@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:36:55 by hthant            #+#    #+#             */
/*   Updated: 2025/02/18 22:52:10 by yocelynnns       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content);

t_list	*ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}

// int main()
// {
//     int *content = malloc(sizeof(int));
//     *content = 42;

//     t_list *node = ft_lstnew(content);

//     if (node)
//     {
//         printf("Node content: %d\n", *(int *)(node->content));
//         free(node->content);
//         free(node);
//     }
//     else
//     {
//         printf("Failed to create node\n");
//     }

//     return 0;
// }
