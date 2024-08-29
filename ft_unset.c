/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 18:36:09 by arekoune          #+#    #+#             */
/*   Updated: 2024/08/29 11:09:00 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_param(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || (str[i] >= ':' && str[i] <= '@') || (str[i] >= '['
				&& str[i] <= '^') || str[i] >= '{' || str[i] == '`')
			return (0);
		i++;
	}
	return (1);
}

t_list	*ft_find(t_list *head, t_list **prev, char *to_unset)
{
	while (head)
	{
		if (str_ncomp(head->str, to_unset, str_len(head->str, '=') - 1) == 0
			&& str_len(head->str, '=') == str_len(to_unset, '\0'))
			return (head);
		*prev = head;
		head = head->next;
	}
	return (NULL);
}

int	ft_unset(t_list **env, char *to_unset)
{
	t_list	*head;
	t_list	*prev;

	head = *env;
	prev = NULL;
	if (!to_unset)
		return (EXIT_SUCCESS);
	if (!check_param(to_unset))
		return (ft_error("unset: `", to_unset, "': not a valid identifier",
				EXIT_FAILURE));
	head = ft_find(head, &prev, to_unset);
	if (head)
	{
		if (!prev)
			*env = head->next;
		else
			prev->next = head->next;
		free(head->str);
		free(head);
		head = NULL;
	}
	return (EXIT_SUCCESS);
}
