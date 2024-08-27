/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:48:20 by haouky            #+#    #+#             */
/*   Updated: 2024/08/27 12:43:50 by haouky           ###   ########.fr       */
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
void herdoc_write(int fd, char *s, t_list *env, int status)
{
    int i;
    char *tmp;
    char *tmp2;
    int j;

    i = 0;
    while (s[i])
    {
        if(s[i] == '$')
        {
            j = 1;
            while (s[i + j] && ((s[i + j] >= '0' && s[i + j] <= '9') || (s[i + j] >= 'a' && s[i + j] <= 'z') || (s[i + j] >= 'A' && s[i + j] <= 'Z')))
                j++;
            if(s[i + 1] == '?')
            {
                tmp2 = ft_itoa(status);
                i++;
            }
            else
            {
                tmp = ft_substr(&s[i], 0, j);
                tmp2 = envv(tmp, env, 0);
                free (tmp);
            }
            write(fd, tmp2, strr_len(tmp2));
            free(tmp2);
            i += j - 1;    
        }
        else
            write(fd, &s[i], 1);
        i++;
    }
}
void thedoc(t_oip *herdoc, int fd, t_list *env, int status)
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
            herdoc_write(fd, s, env, status);
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

int run_here_doc(t_oip *herdoc, t_list *env, int status)
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
            signal(SIGINT,SIG_DFL);
            thedoc(herdoc, fd, env, status);
        }
        wait(&pid);
        herdoc = herdoc->herdoc_next;
        i++;
        if (WIFEXITED(pid)) 
            pid = WEXITSTATUS(pid);
        else if (WIFSIGNALED(pid))
            pid = WTERMSIG(pid) + 128;
    }
    return (pid);
}

