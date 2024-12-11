/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hthant <hthant@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:55:28 by ysetiawa          #+#    #+#             */
/*   Updated: 2024/12/11 17:32:43 by hthant           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*resize_buffer(char *buffer, size_t *current_size, \
size_t total_length)
{
	char	*new_buffer;

	*current_size *= 2;
	new_buffer = malloc(*current_size);
	if (!new_buffer)
	{
		perror("malloc");
		free(buffer);
		return (NULL);
	}
	ft_memcpy(new_buffer, buffer, total_length + 1);
	free(buffer);
	return (new_buffer);
}

int	is_delimiter(const char *line, const char *delimiter, \
size_t delimiter_length)
{
	return (ft_strncmp(line, delimiter, delimiter_length) == 0 \
	&& (line[delimiter_length] == '\n' || line[delimiter_length] == '\0'));
}

char	*read_heredoc(const char *delimiter)
{
	char	*content;
	size_t	total_length;
	size_t	delimiter_length;
	size_t	current_size;
	ssize_t	bytes_read;
	char	*final_content;

	content = malloc(INITIAL_SIZE);
	total_length = 0;
	delimiter_length = ft_strlen(delimiter);
	current_size = INITIAL_SIZE;
	if (!content)
		return (perror("malloc"), NULL);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		bytes_read = read(STDIN_FILENO, content + total_length, \
		current_size - total_length - 1);
		if (bytes_read < 0)
			return (perror("read"), free(content), NULL);
		content[total_length + bytes_read] = '\0';
		if (is_delimiter(content + total_length, delimiter, delimiter_length))
			break ;
		total_length += bytes_read;
		if (total_length + 1 >= current_size)
		{
			content = resize_buffer(content, &current_size, total_length);
			if (!content)
				return (NULL);
		}
	}
	final_content = malloc(total_length + 1);
	if (!final_content)
		return (perror("malloc"), free(content), NULL);
	ft_memcpy(final_content, content, total_length + 1);
	free(content);
	return (final_content);
}
