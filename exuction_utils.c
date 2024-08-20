/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exuction_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 10:18:25 by haouky            #+#    #+#             */
/*   Updated: 2024/08/19 13:19:07 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int run_builtins(char **s, t_list *env)
// {
//     if(str_ncomp(s[0],"echo",5))
//         return (1);
//     else if(str_ncomp(s[0],"pwd",4))
//         return (2);
//     else if(str_ncomp(s[0],"export", 7))
//         return (3);
//     else if(str_ncomp(s[0],"env", 4))
//         return (4);
//     else if(str_ncomp(s[0],"exit", 6))
//         return (5);
//     else if(str_ncomp(s[0],"cd", 3))
//         return (6);
//     return (0);
// }

void first_cmd(t_list *env, t_excution *exuction)
{
    int read;
    int write;
    int fd[2];

    read = 0;
    write = 1;
    env = 0;
    while (exuction->input)
    {
        read = open(exuction->input->name, O_RDONLY);
        if(read == -1)
        {
            printf("minishel: %s : No such file or directory",exuction->input->name);
            exit(1);
        }
        exuction->input = exuction->input->next;
    }
    if(read)
        dup2(read, 0);
    if(exuction->pipe)
        pipe(fd);
}
