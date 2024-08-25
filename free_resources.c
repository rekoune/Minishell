#include "minishell.h"

void	free_in_out(t_oip *list)
{
	t_oip *node;

	while(list)
	{
		free(list->name);
		if(list->type == HERE_DOC)
			free(list->s);
		node = list->next;
		free(list);
		list = node;
	}
}

void	free_lexer(t_lexer_list *list)
{
	t_lexer_list *node;

	while(list)
	{
		free(list->content);
		node = list->next;
		free(list);
		list = node;
	}
}

void	free_resources(t_execution *execution)
{
	t_execution *node;


	while(execution)
	{
		fr_double(execution->cmd);
		free_in_out(execution->input);
		free_in_out(execution->output);
		free(execution->path);
		node = execution->next;
		free(execution);
		execution = node;
	}
}