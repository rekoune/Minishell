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

int main(int ac, char **av, char **env)
{
	char		*str;
	t_lexer_list	*cmd;
	t_excution *exuction;
	int	 i;
	char *type;
	t_list *enva;
	(void) ac;
	(void) av;

	enva = get_env(env);
	while (1)
	{
		str = readline("\033[32mminishell \033[0m> ");
		if (str[0])
		{
		add_history(str);
		cmd = is_tokenized(str);
		print(cmd);
		exuction = parse(cmd,get_env(env));

		}
		printf("---------------------------------------------------------------------\n");
		while (exuction)
		{
			printf("******************************************************************\n");
			i = 0;
			while(exuction->cmd[i])
				printf("exection>>>>>>>>>>>> == %s\n", exuction->cmd[i++]);
			printf("exuction->path >>>>>>>>> ==%s\n ",exuction->path);
			while (exuction->input)
			{
				// printf("/*/checkin \n");
				// if(exuction->input->type == REDIR_IN|| exuction->input->type == HERE_DOC)
				// 	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>in\n");
				// printf("/*/endcheckin \n");
				type = n_type(exuction->input->type);
				printf("input type : %s, name == %s\n", type, exuction->input->name);
				exuction->input = exuction->input->next;
			}
			while (exuction->output)
			{
				// printf("/*/checkout \n");
				// if(exuction->output->type == REDIR_OUT || exuction->output->type == DREDIR_OUT)
				// 	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>out\n");
				// printf("/*/endcheckout \n");
				type = n_type(exuction->output->type);
				printf("input type : %s, name == %s\n", type, exuction->output->name);
				exuction->output = exuction->output->next;
			}
			printf("pipe == %d\n", exuction->pipe);
			exuction = exuction->next;
		}
	}

}