/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:55:03 by haouky            #+#    #+#             */
/*   Updated: 2024/01/08 12:35:47 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	work(int i, const char *s, va_list ap)
{
	int	a;

	a = 0;
	if (s[i] == 'c')
		a += wrtc(va_arg(ap, int));
	else if (s[i] == 's')
		a += wrts(va_arg(ap, char *));
	else if (s[i] == 'p')
		a += putads(va_arg(ap, unsigned long));
	else if (s[i] == 'd')
		a += pnbr(va_arg(ap, int));
	else if (s[i] == 'i')
		a += pnbr(va_arg(ap, int));
	else if (s[i] == 'u')
		a += upnbr(va_arg(ap, unsigned int));
	else if (s[i] == 'X')
		a += hexuper(va_arg(ap, unsigned int));
	else if (s[i] == 'x')
		a += hex(va_arg(ap, unsigned int));
	else if (s[i] == '%')
		a += wrtc('%');
	else
		a += wrtc(s[i]);
	return (a);
}

int	ft_printf(const char *s, ...)
{
	int		i;
	va_list	ap;
	int		a;

	a = 0;
	i = 0;
	va_start(ap, s);
	while (s[i])
	{
		if (s[i] == '%')
		{
			if (s[i + 1] == 0)
				return (a);
			a += work(++i, s, ap);
		}
		else
			a += wrtc(s[i]);
		i++;
	}
	va_end(ap);
	return (a);
}
