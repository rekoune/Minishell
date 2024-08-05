#include "minishell.h"



int main()
{
	char		*str;
	t_cmd_list	*cmd;
	int i = 0;

	while (1)
	{
	str = readline("\033[32mminishell \033[0m> ");
	printf("%s\n", str);
	cmd = get_commands(str);
	while (cmd)
	{
		i = 0;
		printf("================================================\n");
		while (cmd->command[i])
			printf("|%s|\n", cmd->command[i++]);
		cmd = cmd->next;
	}
	printf("aaaaaaaaaaaaaaaaaaaaa\n");
	}

}