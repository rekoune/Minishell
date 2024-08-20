/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 10:33:45 by arekoune          #+#    #+#             */
/*   Updated: 2024/08/19 11:07:24 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	ft_read(char **line, int fd)
{
	int		nf;
	int		num_read;
	char	*bufer;

	nf = 0;
	while (!nf)
	{
		bufer = malloc(BUFFER_SIZE + 1);
		if (!bufer)
		{
			free(*line);
			*line = NULL;
			return ;
		}
		num_read = read(fd, bufer, BUFFER_SIZE);
		bufer[num_read] = '\0';
		nf = check_line(bufer);
		*line = strr_join(*line, bufer);
		if (!(*line))
			return ;
		if (num_read == 0)
			return ;
	}
}

char	*join_reste(char *line, char *reste)
{
	char	*str;
	int		len;

	if (!reste)
		return (NULL);
	len = strr_len(reste);
	str = malloc(len + 1);
	if (!str)
	{
		free(line);
		return (NULL);
	}
	len = 0;
	while (reste[len])
	{
		str[len] = reste[len];
		len++;
	}
	str[len] = '\0';
	free(line);
	return (str);
}

char	*reture_line(char *line, char **reture)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!line)
		return (*reture = NULL);
	while (line[i] && line[i] != '\n')
		i++;
	if (!line[i])
		i--;
	*reture = malloc(i + 2);
	if (!(*reture))
		return (0);
	while (j <= i)
	{
		(*reture)[j] = line[j];
		j++;
	}
	(*reture)[j] = '\0';
	return (&(line[i + 1]));
}

char	*get_next_line(int fd)
{
	static char	*line;
	char		*reture;
	char		*reste;

	if (BUFFER_SIZE <= 0 || read(fd, NULL, 0) == -1)
	{
		free(line);
		line = NULL;
		return (NULL);
	}
	ft_read(&line, fd);
	reste = reture_line(line, &reture);
	if (!reste || !line[0])
	{
		free(line);
		line = NULL;
		free(reture);
		return (NULL);
	}
	line = join_reste(line, reste);
	return (reture);
}
