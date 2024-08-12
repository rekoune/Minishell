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
	new->prev = node;
}

t_lexer_list *new_node(char *str)
{
	t_lexer_list	*node;

	node = malloc(sizeof(t_lexer_list));
	if(!node)
		return(NULL);
	node->content = str;
	node->next = NULL;
	node->prev = NULL;
	return(node);
}
