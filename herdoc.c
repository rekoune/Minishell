/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:48:20 by haouky            #+#    #+#             */
/*   Updated: 2024/08/24 12:44:18 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


t_oip  *get_here_doc(t_execution *execution)
{
    t_oip *inp;
    t_oip *sinp;
    t_oip *firstehrdoc;

    sinp = 0;
    firstehrdoc = 0;
    while (execution)
    {
        inp = execution->input;
        while (inp)
        {
            if(inp->type == HERE_DOC)
            {
                if(sinp)
                    sinp->herdoc_next = inp;
                else
                    firstehrdoc = inp;
                sinp = inp;   
            }
            inp = inp->next;
        }
        execution = execution->next;
    }
    return (firstehrdoc);
}

void thedoc(t_oip *herdoc)
{
    char *dlm;
    char *s;
    
    dlm = str_join(herdoc->name, "\n");
    write(1,">",1);
    s = get_next_line(0);
     while (s)
    {
        if(!str_ncomp(s, dlm,strr_len(dlm)))
        {
            break;
        }
        if(!herdoc->next)
            write(herdoc->fd, s, strr_len(s));
        free(s);
        write(1,">",1);
        s = get_next_line(0);
    } 
    free(dlm);
    if(!herdoc->next)
        close(herdoc->fd);
    exit(0); 
}

void handl(int l)
{
	l = 130;
	exit(l);
}
int run_here_doc(t_oip *herdoc)
{
    char *tmp;
    char *s;
    int pid;
    int i;

    i = 0;
    pid = 0;
    while (herdoc && !pid)
    {
        herdoc->fd = -1;
        tmp = ft_itoa(i);
        s = str_join("/tmp/",tmp);
        free(tmp);
        if(!herdoc->next)
           herdoc->fd = open(s, O_RDWR | O_CREAT | O_TRUNC, 0640);
        printf("herfd %d\n", herdoc->fd);
        free (s);
        pid = fork();
        if(pid == -1)
        {
            perror("minishel:");
            return (-1);
        }
        if(pid == 0)
        {
            signal(SIGINT,handl);
            thedoc(herdoc);
        }
        wait(&pid);
        herdoc = herdoc->herdoc_next;
        i++;
        pid = WEXITSTATUS(pid);
    }
    return (pid);
}

