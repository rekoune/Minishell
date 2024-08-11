#include "minishell.h"

void print(t_lexer_list *head)
{
	char *type;
	char *state;

	printf(" content       len          type                state\n");
		printf("=========================================================\n");
	while (head)
	{
		state = n_state(head->state);
		type = n_type(head->type);
		
		printf("|%s|         %d,           %s,             %s\n",head->content, head->len, type, state);
		head = head->next;
	}
}

int main()
{
	char		*str;
	t_lexer_list	*cmd;

	while (1)
	{
		str = readline("\033[32mminishell \033[0m> ");
		add_history(str);
		cmd = is_tokenized(str);
		print(cmd);
	}

}