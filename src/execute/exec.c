/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:08:26 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/03 21:19:57 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int execute_command(t_ast_node *ast)
{
    if (ast->type == AST_COMMAND) 
    {
        if (strcmp(ast->command->args[0], "echo") == 0)
            return ft_echo(ast->command->args);
        else if (strcmp(ast->command->args[0], "pwd") == 0)
            return ft_pwd();
        else if (strcmp(ast->command->args[0], "exit") == 0)
        {
            free(ast->command->args[0]);
            return (1);
        }
        else if (strcmp(ast->command->args[0], "clear") == 0)
        {
            printf("\033[H\033[J");
            return (0);
        }
    }
    return -1;
}
