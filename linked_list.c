/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 12:56:22 by haouky            #+#    #+#             */
/*   Updated: 2024/08/17 10:47:38 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_back_lst(t_list **head, t_list *new)
{
	t_list	*node;

	node = *head;
	if (!head)
		return ;
	if (*head == NULL)
	{
		(*head) = new;
		return ;
	}
	while (node->next)
		node = node->next;
	node->next = new;
}

void	fadd_back_lst(t_oip **head, t_oip *new)
{
	t_oip	*node;

	node = *head;
	if (!head)
		return ;
	if (*head == NULL)
	{
		(*head) = new;
		return ;
	}
	while (node->next)
		node = node->next;
	node->next = new;
}

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

t_list	*lst_new(char *s)
{
	t_list	*lst;

	lst = malloc(sizeof(t_list));
	if (!lst)
		return (NULL);
	lst->str = s;
	lst->next = NULL;
	return (lst);
}

t_oip	*flst_new(char *s)
{
	t_oip	*lst;

	lst = malloc(sizeof(t_oip));
	if (!lst)
		return (NULL);
	lst->name = s;
	lst->next = NULL;
	lst->herdoc_next = NULL;
	return (lst);
}
