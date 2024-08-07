/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 08:52:12 by haouky            #+#    #+#             */
/*   Updated: 2024/08/07 08:53:44 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	add_back(t_cmd_list **head, t_cmd_list *new)
// {
// 	t_cmd_list *node;

// 	node = *head;
// 	if(!head)
// 		return;
// 	if(*head == NULL)
// 	{
// 		*head = new;
// 		return;
// 	}
// 	while(node->next)
// 		node = node->next;
// 	node->next = new;
// }

// t_cmd_list *new_node(char **str)
// {
// 	t_cmd_list	*node;

// 	node = malloc(sizeof(t_cmd_list));
// 	if(!node)
// 		return(NULL);
// 	node->command = str;
// 	node->next = NULL;
// 	return(node);
// }

char	**getarray(t_list *lst)
{
	char	**str;
	int		i;

	i = 0;
	str = malloc((ft_lstsize(lst) + 1) * sizeof(char *));
	if (!str)
		return (0);
	while (lst)
	{
		str[i] = lst->str;
		i++;
		lst = lst->next;
	}
	str[i] = 0;
	return (str);
}
