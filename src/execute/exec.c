/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:08:26 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/08 14:30:42 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Function to read heredoc content
char *read_heredoc(const char *delimiter)
{
    char *content = malloc(INITIAL_SIZE);
    if (!content)
    {
        perror("malloc");
        return NULL;
    }

    size_t total_length = 0;
    size_t delimiter_length = ft_strlen(delimiter);
    size_t current_size = INITIAL_SIZE;

    while (1)
    {
        // Print prompt for heredoc input
        write(STDOUT_FILENO, "> ", 2);

        // Read a line from standard input
        ssize_t bytes_read = read(STDIN_FILENO, content + total_length, current_size - total_length - 1);
        if (bytes_read < 0) {
            perror("read");
            free(content);
            return NULL;
        }

        // Null-terminate the buffer
        content[total_length + bytes_read] = '\0';

        // Check if the line matches the delimiter
        if (ft_strncmp(content + total_length, delimiter, delimiter_length) == 0 &&
            (content[total_length + delimiter_length] == '\n' || content[total_length + delimiter_length] == '\0')) {
            break; // Exit if the delimiter is found
        }

        // Update total length
        total_length += bytes_read;

        // Check if we need to expand the buffer
        if (total_length + 1 >= current_size) {
            // Double the size of the buffer
            current_size *= 2;
            char *new_content = malloc(current_size);
            if (!new_content) {
                perror("malloc");
                free(content);
                return NULL;
            }
            // Copy the existing content to the new buffer
            ft_memcpy(new_content, content, total_length + 1); // +1 for the null terminator
            free(content);
            content = new_content;
        }
    }

    // Resize the content to fit the actual data
    char *final_content = malloc(total_length + 1);
    if (!final_content) {
        perror("malloc");
        free(content);
        return NULL;
    }
    ft_memcpy(final_content, content, total_length + 1); // +1 for the null terminator
    free(content); // Free the original buffer

    return final_content; // Return the complete heredoc content
}

char *concat_path(const char *dir, const char *cmd)
{
    // Calculate the length of the new string
    size_t dir_len;
    size_t cmd_len;
    size_t total_len;
    char *full_path;

    dir_len = strlen(dir);
    cmd_len = strlen(cmd);
    total_len = dir_len + cmd_len + 2; // +1 for '/' and +1 for '\0'
    // Allocate memory for the new string
    full_path = malloc(total_len);
    if (!full_path)
        return (NULL);

    // Construct the full path
    strcpy(full_path, dir); // Copy the directory
    strcat(full_path, "/"); // Append the '/'
    strcat(full_path, cmd); // Append the command

    return full_path; // Return the constructed path
}

char *find_executable(const char *cmd)
{
    char *path_env;
    char **dirs;
    char *full_path;
    int i;

    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);
    dirs = ft_split(path_env, ':'); // to split the PATH
    if (!dirs)
        return (NULL);
    full_path = NULL;
    i = 0;
    while (dirs[i] != NULL)
    {
        full_path = concat_path(dirs[i], cmd);
        if (!full_path)
        {
            // Free previously allocated directories
            int j = 0;
            while (dirs[j] != NULL)
            {
                free(dirs[j]);
                j++;
            }
            free(dirs);
            return NULL;
        }

        if (access(full_path, X_OK) == 0) // Check if the file is executable
        {
            // Free the directories array
            int j = 0;
            while (dirs[j] != NULL)
            {
                free(dirs[j]);
                j++;
            }
            free(dirs);
            return full_path; // Return the full path if found
        }

        free(full_path);
        i++;
    }

    // Free the directories array
    int j = 0;
    while (dirs[j] != NULL)
    {
        free(dirs[j]);
        j++;
    }
    free(dirs);
    return NULL; // Not found
}

int execute_command(t_ast_node *ast, char **env)
{
    pid_t pid;
    int fd;
    int status;
    t_ast_node *redirect;

    if (ast->type == AST_COMMAND)
    {
        // simple commands
        pid = fork();
        if (pid == 0) // child process
        {
            // Handle redirections
            if (ast->command->redirect)
            {
                redirect = ast->command->redirect;
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

            // Handle heredoc content
            if (ast->command->heredoc)
            {
                int pipefd[2];
                pipe(pipefd);
                write(pipefd[1], ast->command->heredoc, strlen(ast->command->heredoc));
                close(pipefd[1]);
                dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to heredoc
                close(pipefd[0]);
            }


            // Check if the command is an absolute path
            char *command = ast->command->args[0];
            if (command[0] == '/') // Absolute path
            {
                if (access(command, X_OK) == 0) // Check if the file is executable
                {
                    execve(command, ast->command->args, env);
                }
                else
                {
                    fprintf(stderr, "Command not found: %s\n", command);
                    exit(EXIT_FAILURE);
                }
            }
            else // Relative path or command in PATH
            {
                char *executable_path = find_executable(command);
                if (!executable_path)
                {
                    fprintf(stderr, "Command not found: %s\n", command);
                    exit(EXIT_FAILURE);
                }

                execve(executable_path, ast->command->args, env);
                perror("execve");
                free(executable_path);
                exit(EXIT_FAILURE);
            }
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
        pid_t pid1;
        pid_t pid2;

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
            execute_command(ast->pipeline->left, env);
            exit(0);
        }

        // exec right command
        pid2 = fork();
        if (pid2 == 0) // child process for right
        {
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[1]);
            close(pipefd[0]);
            execute_command(ast->pipeline->right, env);
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
