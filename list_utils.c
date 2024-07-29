#include "minishell.h"

void	add_back(t_cmd_list **head, t_cmd_list *new)
{
	t_cmd_list *node;

	node = *head;
	if(!head)
		return;
	if(*head == NULL)
	{
		*head = new;
		return;
	}
	while(node->next)
		node = node->next;
	node->next = new;
}

t_cmd_list *new_node(char **str)
{
	t_cmd_list	*node;

	node = malloc(sizeof(t_cmd_list));
	if(!node)
		return(NULL);
	node->command = str;
	node->next = NULL;
	return(node);
}