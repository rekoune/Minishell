#include "minishell.h"


t_cmd_list	*get_commands(char *str)
{
	t_cmd_list	*head;
	t_cmd_list	*node;
	char		**commands;
	int			i;

	i = 0;
	commands = ft_split(str, '|');
	free(str);
	while(commands[i])
	{
		node = new_node(parcing(commands[i], ' '));
		add_back(&head, node);
		i++; 
	}
	return(head);
}

int main()
{
	char		*str;
	t_cmd_list	*cmd;
	int i = 0;

	while (1)
	{
	str = readline("\033[32mminishell \033[0m> ");
	cmd = get_commands(str);
	while (cmd)
	{
		i = 0;
		printf("================================================\n");
		while (cmd->command[i])
			printf("|%s|\n", cmd->command[i++]);
		cmd = cmd->next;
	}
	}

}