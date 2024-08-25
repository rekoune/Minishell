/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 13:49:10 by haouky            #+#    #+#             */
/*   Updated: 2024/01/08 12:37:28 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int	ft_printf(const char *s, ...);
int	hex(unsigned int n);
int	hexuper(unsigned int n);
int	upnbr(unsigned int n);
int	putads(unsigned long n);
int	pnbr(int nb); 
int	wrtc(char c);
int	wrts(char *c);

#endif