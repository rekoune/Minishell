/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:01:22 by arekoune          #+#    #+#             */
/*   Updated: 2024/08/28 18:35:36 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*get_env(char **env)
{
	int		i;
	t_list	*head;

	i = 0;
	if (!env)
		return (NULL);
	head = NULL;
	while (env && env[i])
	{
		add_back_lst(&head, lst_new(str_ncopy(env[i], str_len(env[i], '\0'))));
		i++;
	}
	return (head);
}

int	is_empty(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

void	ft_replace(t_list **env, char *to_export)
{
	t_list	*node;

	if (is_exist(*env, &node, to_export) == 1)
	{
		free(node->str);
		node->str = str_ncopy(to_export, str_len(to_export, '\0'));
	}
	else
		add_back_lst(env, lst_new(str_ncopy(to_export, str_len(to_export,
						'\0'))));
}

int	is_exist(t_list *head, t_list **node, char *to_export)
{
	while (head)
	{
		if (str_ncomp(head->str, to_export, str_len(to_export, '=')) == 0)
		{
			*node = head;
			return (1);
		}
		head = head->next;
	}
	return (0);
}

int	ft_export(t_list **env, char *to_export)
{
	int	i;

	i = 0;
	if (to_export[0] != '_' && (to_export[0] < 'A' || to_export[0] > 'Z')
		&& (to_export[0] < 'a' || to_export[0] > 'z'))
	{
		ft_printf("minishell : export: `%s': not a valid identifier\n",
			to_export);
		return (EXIT_FAILURE);
	}
	while (to_export && to_export[i] && to_export[i] != '=')
	{
		if (to_export[i] < '0' || (to_export[i] >= ':' && to_export[i] <= '@')
			|| (to_export[i] >= '[' && to_export[i] <= '^')
			|| to_export[i] >= '{' || to_export[i] == '`')
		{
			ft_printf("minishell : export: `%s': not a valid identifier\n",
				to_export);
			return (EXIT_FAILURE);
		}
		i++;
	}
	ft_replace(env, to_export);
	return (EXIT_SUCCESS);
}
