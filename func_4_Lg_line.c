/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func_4_Lg_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 18:09:42 by haouky            #+#    #+#             */
/*   Updated: 2024/08/31 18:15:43 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	alphan(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A'
			&& c <= 'Z'))
		return (1);
	return (0);
}

int	redpip(t_lexer_list *lxr)
{
	if ((lxr->state != GENERAL) || (lxr->type != ' ' && lxr->type != '|'
			&& lxr->type != '<' && lxr->type != '>' && lxr->type != HERE_DOC
			&& lxr->type != DREDIR_OUT))
		return (1);
	return (0);
}
