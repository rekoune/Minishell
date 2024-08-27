/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:01:22 by arekoune          #+#    #+#             */
/*   Updated: 2024/08/27 14:47:43 by arekoune         ###   ########.fr       */
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

int	is_empty(char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		if(str[i] == '=')
			return (0);
		i++;
	}
	return(1);
}

int	ft_env(t_list *env, int fd, int flag)
{
	while(env)
	{
		if(flag == 1)
		{
			ft_write("declare -x ", fd);
			ft_write(env->str, fd);
			ft_write("\n", fd);	
		}
		else
			if(!is_empty(env->str))
			{
				ft_write(env->str, fd);
				ft_write("\n", fd);	
			}
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

int	ft_export(t_list **env, char *to_export)
{
	int	i;
	t_list *node;

	i = 0;
	if(to_export[0] != '_' && (to_export[0] < 'A' || 
		to_export[0] > 'Z') && (to_export[0] < 'a' || to_export[0] > 'z'))
	{
		ft_printf("minishell : export: `%s': not a valid identifier\n", to_export);
		return(EXIT_FAILURE);
	}
	while(to_export && to_export[i] && to_export[i] != '=')
	{
		if(to_export[i] < '0' || (to_export[i] >= ':' && to_export[i] <= '@') || 
			(to_export[i] >= '[' && to_export[i] <= '^') || 
			to_export[i] >= '{' || to_export[i] == '`')
		{
			ft_printf("minishell : export: `%s': not a valid identifier\n", to_export);
			return(EXIT_FAILURE);
		}
		i++;
	}
	if(is_exist(*env, &node, to_export) == 1)
	{
		free(node->str);
		node->str = str_ncopy(to_export, str_len(to_export, '\0'));
	}
	else
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
		ft_printf("minishell : unset: `%s': not a valid identifier\n", to_unset);
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
		if(!s)
		{
			ft_printf("minishell: cd: HOME not set\n");
			return(EXIT_FAILURE);
		}
		if (chdir(s) == -1)
		{
			ft_printf("minishell : cd: %s: No such file or directory\n", s);
			free(s);
			return(EXIT_FAILURE);
		}
	}
	else if (chdir(str) == -1)
	{
		ft_printf("minishell : cd: %s: No such file or directory\n", str);
		return(EXIT_FAILURE);
	}
	free(s);
	return (EXIT_SUCCESS);
}


long	ft_atoi(char *str)
{
	int		i;
	long	nb;
	int		sign;

	i = 0;
	sign = 1;
	nb = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if(str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		if (str[i] > '9' || str[i] < '0')
		{
			ft_printf("minishell: exit: %s: numeric argument required\n", str);
			exit(255);
		}
		nb = nb * 10;
		nb = nb + (str[i++] - 48);
	}
	nb *= sign;
	return (nb);
}

int ft_exit(char **arg)
{
	int status;

	status = 0;
	printf("exit\n");
	if(!arg[0])
		exit(status);
	status = ft_atoi(arg[0]);
	if(arg[1])
		ft_printf("minishell: exit: too many arguments\n");
	else
		exit(status);
	return(EXIT_FAILURE);
}
