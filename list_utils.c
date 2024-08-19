/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:01:50 by arekoune          #+#    #+#             */
/*   Updated: 2024/08/18 11:42:58 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_back(t_lexer_list **head, t_lexer_list *new)
{
	t_lexer_list *node;
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

t_lexer_list *new_node(char *str)
{
	t_lexer_list	*node;

	node = malloc(sizeof(t_lexer_list));
	if(!node)
		return(NULL);
	node->content = str;
	node->next = NULL;
	return(node);
}
