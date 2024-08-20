#include "minishell.h"

int g_status = 1;

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
	// int	 i;
	// char *type;
	t_list *enva;
	(void) ac;
	(void) av;
	int pid;

	enva = get_env(env);
	while (1)
	{
		str = readline("\033[32mminishell \033[0m> ");
		if(str)
		{ 
			add_history(str);
			cmd = is_tokenized(str);
			exuction = parse(cmd,get_env(env));
			printf("---------------------------------------------------------------------\n");
			pid = fork();
			printf("pid = %d\n",pid);
			if(!pid)
			{
				printf("in\n");
				run_exuction(exuction, enva);
				exit(0);
			}
			 wait(&g_status);
			printf("status= %d\n",g_status);
		}
		
	}

}