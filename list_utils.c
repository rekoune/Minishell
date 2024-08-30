/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:01:50 by arekoune          #+#    #+#             */
/*   Updated: 2024/08/30 17:12:59 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_back(t_lexer_list **head, t_lexer_list *new)
{
	t_lexer_list	*node;

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

t_lexer_list	*new_node(char *str)
{
	t_lexer_list	*node;

	node = malloc(sizeof(t_lexer_list));
	if (!node)
		return (NULL);
	node->content = str;
	node->next = NULL;
	return (node);
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

int	ft_write(char *str, int fd, int flag)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (write(fd, &str[i++], 1) == -1)
			return (EXIT_FAILURE);
		if (flag == 1 && str[i - 1] == '=')
		{
			write(1, "\"", 1);
			flag = 2;
		}
	}
	if (flag == 2)
		write(1, "\"", 1);
	return (EXIT_SUCCESS);
}
