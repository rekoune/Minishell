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
	t_excution *exuction;
	int	 i = 0;
	while (1)
	{
		str = readline("\033[32mminishell \033[0m> ");
		add_history(str);
		cmd = is_tokenized(str);
		print(cmd);
		exuction = parce(cmd);
		while (exuction)
		{
			while(exuction->cmd[i])
				printf("exection == %s\n", exuction->cmd[i++]);
			while (exuction->action->input)
			{
				printf("input type : %s, name == %s\n", n_type(exuction->action->input->type), exuction->action->input->name);
				exuction->action->input = exuction->action->input->next;
			}
			while (exuction->action->output)
			{
				printf("input type : %s, name == %s\n", n_type(exuction->action->output->type), exuction->action->output->name);
				exuction->action->output = exuction->action->output->next;
			}
			printf("pipe == %d\n", exuction->action->pipe);
			exuction = exuction->next;
		}
	}

}