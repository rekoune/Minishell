/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 11:33:26 by arekoune          #+#    #+#             */
/*   Updated: 2024/08/29 11:33:48 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_error(char *str1, char *str2, char *str3, int exit_status)
{
	ft_write("minishell: ", 2, 0);
	if (str1)
		ft_write(str1, 2, 0);
	if (str2)
		ft_write(str2, 2, 0);
	if (str3)
		ft_write(str3, 2, 0);
	ft_write("\n", 2, 0);
	return (exit_status);
}
