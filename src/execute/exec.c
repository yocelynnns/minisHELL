/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysetiawa <ysetiawa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:08:26 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/05 17:35:17 by ysetiawa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int execute_command(t_ast_node *ast)
{
    pid_t pid;
    int fd;
    int status;
    
    if (ast->type == AST_COMMAND) 
    {
        // simple commands
        pid = fork();
        if (pid == 0) // child process
        {
            // redirections
            if (ast->command->redirect)
            {
                t_ast_node *redirect = ast->command->redirect;
                while (redirect)
                {
                    if (redirect->redirect->type == REDIRECT_IN)
                    {
                        fd = open(redirect->redirect->file, O_RDONLY);
                        if (fd < 0)
                        {
                            perror("open");
                            exit(EXIT_FAILURE);
                        }
                        dup2(fd, STDIN_FILENO);
                        close(fd);
                    }
                    else if (redirect->redirect->type == REDIRECT_OUT)
                    {
                        fd = open(redirect->redirect->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if (fd < 0)
                        {
                            perror("open");
                            exit(EXIT_FAILURE);
                        }
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                    }
                    else if (redirect->redirect->type == APPEND)
                    {
                        fd = open(redirect->redirect->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                        if (fd < 0)
                        {
                            perror("open");
                            exit(EXIT_FAILURE);
                        }
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                    }
                    redirect = redirect->redirect->next;
                }
            }
            execvp(ast->command->args[0], ast->command->args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("fork");
            return -1;
        }
        else // parent process
        {
            waitpid(pid, &status, 0);
            return WEXITSTATUS(status);
        }
    }
    else if (ast->type == AST_PIPELINE)
    {
        // pipelines
        int pipefd[2];
        pid_t pid1, pid2;

        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            return -1;
        }

        // exec left command
        pid1 = fork();
        if (pid1 == 0) // child process for left
        {
            dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to pipe
            close(pipefd[0]); // close unused read end
            close(pipefd[1]); // close write end after dup
            execute_command(ast->pipeline->left);
            exit(0);
        }

        // exec right command
        pid2 = fork();
        if (pid2 == 0) // child process for right
        {
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[1]);
            close(pipefd[0]);
            execute_command(ast->pipeline->right);
            exit(0);
        }

        // parent process
        close(pipefd[0]); // close read end
        close(pipefd[1]); // close write end
        waitpid(pid1, NULL, 0); // wait for left command
        waitpid(pid2, NULL, 0); // wait for right command
        return 0;
    }
    return -1;
}
