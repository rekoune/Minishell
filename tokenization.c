/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 10:36:48 by arekoune          #+#    #+#             */
/*   Updated: 2024/08/31 16:59:30 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum e_token	add_type(t_lexer_list *node)
{
	if (!check_char(node->content[0]) && node->content[0] != '$')
		return (WORD);
	else if (node->content[0] == ' ' || node->content[0] == '\t')
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

int	get_spec_char(t_lexer_list **list, char *str, int i, int j)
{
	char			spe_char;
	t_lexer_list	*node;

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
	return (add_back(list, node), j);
}

t_lexer_list	*is_tokenized(char *str)
{
	t_lexer_list	*list;
	t_lexer_list	*node;
	int				i;
	int				j;

	i = 0;
	list = NULL;
	while (str[i])
	{
		j = 0;
		if (check_char(str[i + j]))
			i += get_spec_char(&list, str, i, j);
		else
		{
			while (str[i + j] && !check_char(str[i + j]))
				j++;
			node = new_node(str_ncopy(&str[i], j));
			node->len = j;
			add_back(&list, node);
			i += j;
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
