/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 10:33:42 by arekoune          #+#    #+#             */
/*   Updated: 2024/08/19 11:02:37 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
void	ft_read(char **line, int fd);
char	*join_reste(char *line, char *reste);
char	*reture_line(char *line, char **reture);
int		check_line(char *str);
char	*strr_join(char *s1, char *s2);
char	*mini_join(char *s1, char *s2, char *str);
int		strr_len(char *str);

#endif