/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 16:42:52 by arekoune          #+#    #+#             */
/*   Updated: 2024/09/01 09:54:08 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redir_synx(t_lexer_list **head)
{
	while ((*head)->next && (*head)->next->type == WHITE_SPACE)
		(*head) = (*head)->next;
	if ((*head)->next == NULL || (*head)->next->type == PIPE_LINE
		|| (*head)->next->type == REDIR_IN || (*head)->next->type == REDIR_OUT
		|| (*head)->next->type == DREDIR_OUT || (*head)->next->type == HERE_DOC)
		return (1);
	return (0);
}

int	is_redir(enum e_token type)
{
	if (type == REDIR_IN || type == REDIR_OUT || type == DREDIR_OUT
		|| type == HERE_DOC)
		return (1);
	return (0);
}

int	check_pipe(t_lexer_list *head)
{
	while (head && (head->type == WHITE_SPACE && head->state == GENERAL))
		head = head->next;
	if (head && head->type == PIPE_LINE && head->state == GENERAL)
		return (0);
	while (head)
	{
		if (head->type == PIPE_LINE && head->state == GENERAL)
		{
			head = head->next;
			while (head && head->type == WHITE_SPACE)
				head = head->next;
			if (!head || (head->type == PIPE_LINE && head->state == GENERAL))
				return (0);
		}
		head = head->next;
	}
	return (1);
}

int	more_check(t_lexer_list *head)
{
	int	n_quote;

	n_quote = 0;
	if (!check_pipe(head))
		return (error("minishell : syntax error\n"));
	while (head)
	{
		if ((head->type == QOUTE || head->type == DOUBLE_QUOTE)
			&& head->state == GENERAL)
			n_quote++;
		else if (is_redir(head->type) && head->state == GENERAL)
			if (check_redir_synx(&head))
				return (error("minishell : syntax error\n"));
		head = head->next;
	}
	if (n_quote % 2 != 0)
		return (error("minishell : syntax error\n"));
	return (0);
}

int	check_syntax(t_lexer_list *head)
{
	t_lexer_list	*node;

	node = head;
	while (node)
	{
		if (node->type == REDIR_OUT)
		{
			if (node->len == 2)
				node->type = DREDIR_OUT;
			else if (node->len > 2 && node->state == GENERAL)
				return (error("minishell : syntax error\n"));
		}
		else if (node->type == REDIR_IN)
		{
			if (node->len == 2)
				node->type = HERE_DOC;
			else if (node->len > 2 && node->state == GENERAL)
				return (error("minishell : syntax error\n"));
		}
		else if (node->type == PIPE_LINE && node->len > 1
			&& node->state == GENERAL)
			return (error("minishell : syntax error\n"));
		node = node->next;
	}
	return (more_check(head));
}
