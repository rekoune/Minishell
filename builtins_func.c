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
