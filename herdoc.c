/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:48:20 by haouky            #+#    #+#             */
/*   Updated: 2024/08/25 16:47:57 by arekoune         ###   ########.fr       */
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

void thedoc(t_oip *herdoc, int fd)
{
    char *dlm;
    char *s;
    
    dlm = str_join(herdoc->name, "\n");
    write(1,">",1);
    s = get_next_line(0);
     while (s)
    {
        if(!str_ncomp(s, dlm,strr_len(dlm)))
            break;
        if(!herdoc->next)
            write(fd, s, strr_len(s));
        free(s);
        write(1,">",1);
        s = get_next_line(0);
    } 
    free(dlm);
    free(s);
    if(!herdoc->next)
        close(fd);
    exit(0); 
}

void sig_handel(int l)
{
    printf("hona haon hona\n");
	l = 130;
	exit(l);
}
int run_here_doc(t_oip *herdoc)
{
    char *tmp;
    int pid;
    int fd;
    int i;

    i = 0;
    pid = 0;
    while (herdoc && !pid)
    {
        if(!herdoc->next)
        {
            tmp = ft_itoa(i);
            herdoc->s = str_join("/tmp/",tmp);
            free(tmp);
            fd = open(herdoc->s, O_RDWR | O_CREAT | O_TRUNC, 0640);
        }
        pid = fork();
        if(pid == -1)
        {
            perror("minishel:");
            return (-1);
        }
        if(pid == 0)
        {
            signal(SIGINT,sig_handel);
            thedoc(herdoc, fd);
        }
        wait(&pid);
        herdoc = herdoc->herdoc_next;
        i++;
        pid = WEXITSTATUS(pid);
    }
    return (pid);
}

