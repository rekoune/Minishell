/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putads.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 11:12:01 by haouky            #+#    #+#             */
/*   Updated: 2024/01/07 13:52:06 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	putads(unsigned long n)
{
	char	*base;
	int		i;
	char	c[17];
	int		a;

	a = 0;
	i = 0;
	base = "0123456789abcdef";
	a += wrts("0x");
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
