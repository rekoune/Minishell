/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 12:56:22 by haouky            #+#    #+#             */
/*   Updated: 2024/08/07 08:53:17 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "minishell.h"

void    add_back_lst(t_list **head, t_list *new)
{
    t_list *node;
    node = *head;
    if(!head)
        return;
    if(*head == NULL)
    {
        (*head) = new;
        return;
    }
    while(node->next)
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

t_list *lst_new(char *s)
{
	t_list *lst;

	lst = malloc (sizeof(t_list));
	if(!lst)
		return (0);
	lst->str = s;
	lst->next = 0;
	return(lst);	
}

