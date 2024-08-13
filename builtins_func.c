#include "minishell.h"

void	ft_echo(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return;
	while (str[i])
	{
		printf("%s", str[i]);
		i++;
	}
}

t_list	*get_env(char **env)
{
	int		i;
	t_list	*head;

	i = 0;
	head = NULL;
	while(env && env[i])
	{
		add_back_lst(&head, lst_new(str_ncopy(env[i], str_len(env[i], '\0'))));
		i++;
	}
	return(head);
}

int main(int ac, char **av, char **env)
{
	(void) ac; 
	(void) av;
	// t_list *head;
	char *str[3];

	str[0] = "rekoune\n";
	str[1] = "abdellah            ";
	str[2] = NULL;
	// head = get_env(env);
	// while(head)
	// {
	// 	printf("%s\n", head->str);
	// 	head = head->next;
	// }
	ft_echo(str);
}