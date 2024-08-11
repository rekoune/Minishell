#include "minishell.h"

enum e_token	add_type(t_lexer_list *node)
{
	if (!check_char(node->content[0]) && node->content[0] != '$')
		return (WORD);
	else if(node->content[0] ==  ' ')
		return (WHITE_SPACE);
	else if(node->content[0] ==  '\n')
		return (NEW_LINE);
	else if(node->content[0] ==  '\'')
		return (QOUTE);
	else if(node->content[0] ==  '\"')
		return (DOUBLE_QUOTE);
	else if(node->content[0] ==  '\\')
		return (ESCAPE);
	else if(node->content[0] == '$')
		return (ENV);
	else if(node->content[0] ==  '|')
		return (PIPE_LINE);
	else if(node->content[0] ==  '<')
		return (REDIR_IN);
	else if(node->content[0] ==  '>')
		return (REDIR_OUT);
	return(0);
}

void	check_syntax(t_lexer_list *node)
{
	int	n_quote;

	n_quote = 0;
	while(node)
	{
		if (node->type == REDIR_OUT)
		{
			if (node->len == 2)
				node->type = DREDIR_OUT;
			else if (node->len > 2 && node->state == GENERAL)
				error("ERROR : syntax error\n");
		}
		else if (node->type == REDIR_IN)
		{
			if (node->len == 2)
				node->type = HERE_DOC;
			else if (node->len > 2 && node->state == GENERAL)
				error("ERROR : syntax error\n");
		}
		else if (node->type == PIPE_LINE  && node->len > 1 && node->state == GENERAL)
			error("ERROR : syntax error\n");
		else if ((node->type == QOUTE || node->type == DOUBLE_QUOTE) && node->state == GENERAL)
			n_quote++;
		node = node->next;
	}
	if (n_quote % 2 != 0)
		error("ERROR : syntax error\n");
	return;
}

t_lexer_list *set_quote_state (t_lexer_list *head, enum e_token type)
{
	enum e_state state;
	
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
	return(head);
}


void	add_state(t_lexer_list *head)
{
	t_lexer_list *node;
	
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
	check_syntax(node);
}

t_lexer_list	*is_tokenized(char *str)
{
	t_lexer_list 	*list;
	t_lexer_list 	*node;
	char			spe_char;
	int				i;
	int				j;
	
	j = 0;
	i = 0;
	spe_char = ' ';
	list = NULL;
	while (i < str_len(str, '\0') && str[i])
	{
		if(check_char(str[i + j]))
		{
			spe_char = str[i + j];
			while(str[i + j] == spe_char)
			{
				j++;
				if ((spe_char == '\"' || spe_char == '\'') && str[i + j] == spe_char)
					break;
			}
			node = new_node(str_ncopy(&str[i], j));
			node->len = j;
			add_back(&list, node);
			i += j;
			j = 0;
		} 
		else 
		{
			while (!check_char(str[i + j]) && str[i + j])
				j++;
			node = new_node(str_ncopy(&str[i], j));
			node->len = j;
			add_back(&list, node);
			i += j;
			j = 0;
		}
	}
	add_state(list);
	return(list);
}

char *n_state(enum e_state state)
{
	char *str;

	str = NULL;
	if (state == GENERAL)
		str = "GENERAL";
	else if (state == IN_QUOTE)
		str = "IN_QUOTE";
	else if (state == IN_DQUOTE)
		str = "IN_DQUOTE";
	return (str);
}

char *n_type (enum e_token type)
{
	char *str = NULL;

	if (type == WORD)
		str = "WORD";
	else if (type == WHITE_SPACE)
		str = "WHITE_SPACE";
	else if (type == NEW_LINE)
		str = "NEW_LINE";
	else if (type == QOUTE)
		str = "QOUTE";
	else if (type == DOUBLE_QUOTE)
		str = "DOUBLE_QUOTE";
	else if (type == ESCAPE)
		str = "ESCAPE";
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
