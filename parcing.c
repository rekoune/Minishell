/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:16:25 by haouky            #+#    #+#             */
/*   Updated: 2024/08/05 10:24:16 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_list	*get_commands(char *str)
{
	t_cmd_list	*head;
	t_cmd_list	*node;
	char		**commands;
	int			i;

	i = 0;
	commands = ft_split(str, '|');
	free(str);
	while(commands[i])
	{
		node = new_node(ft_split(commands[i], ' '));
		add_back(&head, node);
		i++; 
	}
	return(head);
}
