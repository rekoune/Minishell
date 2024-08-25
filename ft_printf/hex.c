/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 10:12:44 by haouky            #+#    #+#             */
/*   Updated: 2024/01/07 13:51:48 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	hex(unsigned int n)
{
	char	*base;
	int		i;
	char	c[9];
	int		a;

	a = 0;
	i = 0;
	base = "0123456789abcdef";
	if (n == 0)
		a += wrtc(base[0]);
	while (n)
	{
		c[i++] = base[n % 16];
		n /= 16;
	}
	i--;
	while (i >= 0)
	{
		a += wrtc(c[i]);
		i--;
	}
	return (a);
}
