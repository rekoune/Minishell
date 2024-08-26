/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:01:22 by arekoune          #+#    #+#             */
/*   Updated: 2024/08/25 19:44:34 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_write(char *str, int fd)
{
	int i;

	i = 0;
	while(str[i])
	{
		if(write(fd, &str[i++], 1) == -1)
			return(EXIT_FAILURE);
	}
	return(EXIT_SUCCESS);
}
int skip_n(char *str)
{
	int i;
	
	i = 0;
	if(str[i] == '-')
	{
		i++;
		while(str[i] && str[i] == 'n')
			i++;
	}
	if(i == str_len(str, '\0'))
		return(0);
	return(1);
}

int	ft_echo(char **str, int fd)
{
	int	i;
	char flag;

	i = 0;
	flag = 'n';
	if (!str)
		return (EXIT_FAILURE);
	while(str[i] && (!str_ncomp(str[i], "-n", 3) || !skip_n(str[i])))
	{
		flag = 'y';
		i++;
	}
	while (str[i])
	{
		if (ft_write(str[i], fd) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		if(str[i + 1] != NULL)
			if (ft_write(" ", fd) == EXIT_FAILURE)	
				return(EXIT_FAILURE);
		i++;
	}
	if (flag == 'n')
		if (ft_write("\n", fd) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
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

int	ft_pwd(int fd)
{
	char buff[1024];

	if(getcwd(buff, sizeof(buff)) == NULL)
		return (EXIT_FAILURE);
	if (ft_write(buff, fd) == EXIT_FAILURE || ft_write("\n", fd) == EXIT_FAILURE)
		return(EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	ft_env(t_list *env, int fd, int flag)
{
	while(env)
	{
		if(flag == 1)
			ft_write("declare -x ", fd);
		if(ft_write(env->str, fd) == EXIT_FAILURE || ft_write("\n", fd) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		env = env->next;
	}
	return (EXIT_SUCCESS);
}

int	is_exist(t_list *head, t_list **node, char *to_export)
{
	while(head)
	{
		if(str_ncomp(head->str, to_export, str_len(to_export, '=')) == 0)
		{
			*node = head;
			return(1);
		}
		head = head->next;
	}
	return(0);
}

int	ft_export(t_list **env, char *to_export, int fd)
{
	int	flag;
	int	i;
	t_list *node;

	i = 0;
	flag = 0;
	if(!to_export)
		return(ft_env(*env, fd, 1));
	if(to_export[0] != '_' && (to_export[0] < 'A' || 
		to_export[0] > 'Z') && (to_export[0] < 'a' || to_export[0] > 'z'))
	{
		ft_write("minishell : export '", 2);
		ft_write(to_export, 2);
		ft_write("': not a valid identifier\n", 2);
		return(EXIT_FAILURE);
	}
	while(to_export && to_export[i] && to_export[i] != '=')
	{
		if(to_export[i] < '0' || (to_export[i] >= ':' && to_export[i] <= '@') || 
			(to_export[i] >= '[' && to_export[i] <= '^') || 
			to_export[i] >= '{' || to_export[i] == '`')
		{
			ft_write("minishell : export '", 2);
			ft_write(to_export, 2);
			ft_write("': not a valid identifier\n", 2);
			return(EXIT_FAILURE);
		}
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
	return (EXIT_SUCCESS);
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

int	ft_unset(t_list **env, char *to_unset)
{
	t_list *head;
	t_list *prev;

	head = *env;
	prev = NULL;
	if(!to_unset)
		return(EXIT_SUCCESS);
	if (!check_param(to_unset))
	{
		ft_write("minishell : export '", 2);
		ft_write(to_unset, 2);
		ft_write("': not a valid identifier\n", 2);
		return(EXIT_FAILURE);	
	}
	while(head)
	{
		if(str_ncomp(head->str, to_unset, str_len(head->str, '=') - 1) == 0 && str_len(head->str, '=') == str_len(to_unset, '\0'))
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
	return (EXIT_SUCCESS);
}

int	ft_cd(char *str, t_list *env)
{
	char *s;
	
	s = NULL;
	if(!str)
	{
		s = envv("$HOME", env, 0);
		chdir(s);
	}
	else if (chdir(str) == -1)
	{
		ft_write("minishell : cd: ", 2);
		ft_write(str, 2);
		ft_write(": No such file or directory\n", 2);
		return(EXIT_FAILURE);
	}
	free(s);
	return (EXIT_SUCCESS);
}

int ft_exit()
{
	printf("exit\n");
	exit(0);
	return(EXIT_SUCCESS);
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