/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 10:36:48 by arekoune          #+#    #+#             */
/*   Updated: 2024/08/28 10:08:27 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum e_token	add_type(t_lexer_list *node)
{
	if (!check_char(node->content[0]) && node->content[0] != '$')
		return (WORD);
	else if (node->content[0] == ' ')
		return (WHITE_SPACE);
	else if (node->content[0] == '\'')
		return (QOUTE);
	else if (node->content[0] == '\"')
		return (DOUBLE_QUOTE);
	else if (node->content[0] == '$')
		return (ENV);
	else if (node->content[0] == '|')
		return (PIPE_LINE);
	else if (node->content[0] == '<')
		return (REDIR_IN);
	else if (node->content[0] == '>')
		return (REDIR_OUT);
	return (0);
}

int	check_redir_synx(t_lexer_list **head)
{
	if ((*head)->next && (*head)->next->type == WHITE_SPACE)
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

int	more_check(t_lexer_list *head)
{
	int	n_quote;

	n_quote = 0;
	if (head->type == PIPE_LINE)
		return (error("minishell : syntax error\n"));
	while (head)
	{
		if ((head->type == QOUTE || head->type == DOUBLE_QUOTE)
			&& head->state == GENERAL)
			n_quote++;
		else if (is_redir(head->type) && head->state == GENERAL)
			if (check_redir_synx(&head))
				return (error("minishell : syntax error\n"));
		if (head->type == PIPE_LINE && head->state == GENERAL)
		{
			if (head->next && head->next->type == WHITE_SPACE)
				head = head->next;
			if (head->next == NULL)
				return (error("minishell : syntax error\n"));
		}
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

t_lexer_list	*set_quote_state(t_lexer_list *head, enum e_token type)
{
	enum e_state	state;

	state = GENERAL;
	head->state = state;
	if (head->type == QOUTE)
		state = IN_QUOTE;
	else if (head->type == DOUBLE_QUOTE)
		state = IN_DQUOTE;
	head = head->next;
	while (head && head->type != type)
	{
		head->state = state;
		head = head->next;
	}
	if (head)
	{
		head->state = GENERAL;
		head = head->next;
	}
	return (head);
}

void	add_state(t_lexer_list *head)
{
	t_lexer_list	*node;

	node = head;
	while (node)
	{
		node->type = add_type(node);
		node = node->next;
	}
	node = head;
	while (head)
	{
		if (head->type == QOUTE || head->type == DOUBLE_QUOTE)
			head = set_quote_state(head, head->type);
		else
		{
			head->state = GENERAL;
			head = head->next;
		}
	}
}

t_lexer_list	*is_tokenized(char *str)
{
	t_lexer_list	*list;
	t_lexer_list	*node;
	char			spe_char;
	int				i;
	int				j;

	j = 0;
	i = 0;
	spe_char = ' ';
	list = NULL;
	while (str[i])
	{
		if (check_char(str[i + j]))
		{
			spe_char = str[i + j];
			if (spe_char == '$')
			{
				j++;
				if (str[i + j] == '?' || (str[i + 1] >= '0' && str[i + 1] <= '9'))
					j++;
				else
					while (str[i + j] && !check_char(str[i + j]))
						j++;
			}
			else
			{
				while (str[i + j] == spe_char)
				{
					j++;
					if ((spe_char == '\"' || spe_char == '\''))
						break ;
				}
			}
			node = new_node(str_ncopy(&str[i], j));
			node->len = j;
			add_back(&list, node);
			i += j;
			j = 0;
		}
		else
		{
			while (str[i + j] && !check_char(str[i + j]))
				j++;
			node = new_node(str_ncopy(&str[i], j));
			node->len = j;
			add_back(&list, node);
			i += j;
			j = 0;
		}
	}
	add_state(list);
	return (list);
}

char	*n_state(enum e_state state)
{
	char	*str;

	str = NULL;
	if (state == GENERAL)
		str = "GENERAL";
	else if (state == IN_QUOTE)
		str = "IN_QUOTE";
	else if (state == IN_DQUOTE)
		str = "IN_DQUOTE";
	return (str);
}

char	*n_type(enum e_token type)
{
	char	*str;

	str = NULL;
	if (type == WORD)
		str = "WORD";
	else if (type == WHITE_SPACE)
		str = "WHITE_SPACE";
	else if (type == QOUTE)
		str = "QOUTE";
	else if (type == DOUBLE_QUOTE)
		str = "DOUBLE_QUOTE";
	else if (type == ENV)
		str = "ENV";
	else if (type == PIPE_LINE)
		str = "PIPE_LINE";
	else if (type == REDIR_IN)
		str = "REDIR_IN";
	else if (type == REDIR_OUT)
		str = "REDIR_OUT";
	else if (type == DREDIR_OUT)
		str = "DREDIR_OUT";
	else if (type == HERE_DOC)
		str = "HERE_DOC";
	return (str);
}
