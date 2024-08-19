/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:01:22 by arekoune          #+#    #+#             */
/*   Updated: 2024/08/18 11:43:38 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(char **str, char flag)
{
	int	i;

	i = 0;
	if (!str)
		exit(0);
	while (str[i])
	{
		printf("%s", str[i]);
		i++;
	}
	if (flag == 'n')
		printf("\n");
	exit(0);
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

void	ft_pwd()
{
	char buff[1024];

	if(getcwd(buff, sizeof(buff)) == NULL)
		error("ERROR : getwcd fails!!");
	printf("%s\n", buff);
	exit(0);
}

void	ft_env(t_list *env)
{
	while(env)
	{
		printf("%s\n", env->str);
		env = env->next;
	}
	exit(0);
}

int	is_exist(t_list *head, t_list **node, char *to_export)
{
	while(head)
	{
		if(str_ncomp(head->str, to_export, str_len(to_export, '=') - 1) == 0)
		{
			*node = head;
			return(1);
		}
		head = head->next;
	}
	return(0);
}

void	ft_export(t_list **env, char *to_export)
{
	int	flag;
	int	i;
	t_list *node;

	i = 0;
	flag = 0;
	if(to_export[0] != '_' && (to_export[0] < 'A' || 
		to_export[0] > 'Z') && (to_export[0] < 'a' || to_export[0] > 'z'))
		error("ERROR : not a valid identifier!!");
	while(to_export && to_export[i] && to_export[i] != '=')
	{
		if(to_export[i] < '0' || (to_export[i] >= ':' && to_export[i] <= '@') || 
			(to_export[i] >= '[' && to_export[i] <= '^') || 
			to_export[i] >= '{' || to_export[i] == '`')
			error("Error : invalid parameter name");
		i++;
	}
	if(to_export[i] == '=')
		flag = 1;
	if(flag == 1 && is_exist(*env, &node, to_export) == 1)
	{
		free(node->str);
		node->str = str_ncopy(to_export, str_len(to_export, '\0'));
	}
	else if (flag == 1)
		add_back_lst(env, lst_new(str_ncopy(to_export, str_len(to_export, '\0'))));
	exit(0);
}

int	check_param(char *str)
{
	int i;

	i = 0;
	if(!str)
		return(1);
	while(str[i])
	{
		if(str[i] < '0' || (str[i] >= ':' && str[i] <= '@') || (str[i] >= '[' && str[i] <= '^') || str[i] >= '{' || str[i] == '`')
			return(0);
		i++;
	}
	return(1);
}

void	ft_unset(t_list **env, char *to_unset)
{
	t_list *head;
	t_list *prev;

	head = *env;
	prev = NULL;
	if (!check_param(to_unset))
		error("Error : invalid parameter name");
	while(head)
	{
		if(str_ncomp(head->str, to_unset, str_len(to_unset, '\0') - 1) == 0)
			break;
		prev = head;
		head = head->next;
	}
	if(head)
	{
		if (!prev)
			*env = head->next;
		else
			prev->next = head->next;
		free(head->str);
		free(head);
	}
	exit(0);
}

void	ft_cd(char *str)
{
	if(chdir(str) == -1)
		error("ERROR : cd function fails!!");
	exit(0);
}

void ft_exit()
{
	exit(0);
}

// int main(int ac, char **av, char **env)
// {
// 	t_list *head;
// 	(void) ac;
// 	(void)	av;
// 	head = get_env(env);
// 	ft_env(head);
// 	printf("==============================================================================\n");
// 	ft_env(head);
// 	sleep(3);
// 	ft_export(&head, "abde=ls");
// 	ft_env(head);
// 	sleep(3);
// 	printf("==============================================================================\n");
// 	ft_export(&head, "abde=rekoune");
// 	ft_env(head);
// 	sleep(3);
// 	ft_export(&head, "hamza=aouky");
// 	ft_env(head);
// 	printf("==============================================================================\n");
// 	sleep(3);
// 	ft_unset(&head, "abde");
// 	ft_env(head);
// 	sleep(3);
// 	ft_unset(&head, "hamza");
// 	ft_env(head);
// 	sleep(3);
// 	ft_unset(&head, "LESS");
// 	ft_env(head);
	
// 	ft_pwd();
// 	ft_cd("../pipex");
// 	ft_pwd();
// }