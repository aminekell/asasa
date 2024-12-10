/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akella <akella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 19:42:45 by akella            #+#    #+#             */
/*   Updated: 2024/12/04 03:11:59 by akella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*trim_buffer(char *buffer)
{
	int		i;
	int		j;
	char	*new_buffer;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	new_buffer = ft_calloc(ft_strlen(buffer) - i + 1, sizeof(char));
	if (!new_buffer)
		return (NULL);
	i++;
	j = 0;
	while (buffer[i])
		new_buffer[j++] = buffer[i++];
	free(buffer);
	return (new_buffer);
}

char	*extract_line(char *buffer)
{
	char	*line;
	int		i;
	char	*newline_pos;

	newline_pos = ft_strchr(buffer, '\n');
	if (!buffer || !*buffer)
		return (NULL);
	if (newline_pos)
		i = newline_pos - buffer;
	else
		i = ft_strlen(buffer);
	line = ft_calloc(i + 2, sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*free_content(char **content, char **buffer)
{
	if (!*content)
	{
		*content = ft_calloc(1, 1);
		if (!*content)
		{
			free(*buffer);
			return (NULL);
		}
	}
	return (*content);
}

char	*read_and_append(int fd, char *content)
{
	char	*buffer;
	int		bytes_read;
	char	*comi;

	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer)
		return (NULL);
	if (!free_content(&content, &buffer))
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(buffer, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		buffer[bytes_read] = '\0';
		comi = ft_strjoin(content, buffer);
		free(content);
		content = comi;
	}
	free(buffer);
	return (content);
}

char	*get_next_line(int fd)
{
	static char	*remainder[1024];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		if (remainder[fd] != NULL)
		{
			free(remainder[fd]);
			remainder[fd] = NULL;
		}
		return (NULL);
	}
	remainder[fd] = read_and_append(fd, remainder[fd]);
	if (!remainder[fd])
		return (NULL);
	line = extract_line(remainder[fd]);
	remainder[fd] = trim_buffer(remainder[fd]);
	return (line);
}
