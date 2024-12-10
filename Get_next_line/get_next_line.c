/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akella <akella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 19:42:45 by akella            #+#    #+#             */
/*   Updated: 2024/12/09 11:20:28 by akella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
				content = NULL;
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

	buffer = ft_calloc((size_t) BUFFER_SIZE + 1, sizeof(char));
	if (!buffer)
		return (NULL);
	if (!free_content(&content, &buffer))
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(buffer, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if(bytes_read < 0)
		{
			free(content);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		comi = ft_strjoin(content, buffer);
		// if(!comi)
		// 	break ;
		free(content);
			
		content = comi;
	}
	free(buffer);
	return (content);
}

char	*get_next_line(int fd)
{
	static char	*remainder;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX || read(fd, 0, 0))
		return (free(remainder), NULL);
	remainder = read_and_append(fd, remainder);
	if (!remainder)
		return (NULL);
	line = extract_line(remainder);
	remainder = trim_buffer(remainder);
	return (line);
}
