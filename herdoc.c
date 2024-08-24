/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:48:20 by haouky            #+#    #+#             */
/*   Updated: 2024/08/24 10:43:47 by arekoune         ###   ########.fr       */
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

void run_here_doc(t_oip *herdoc)
{
    char *tmp;
    char *s;
    char *dlm;
    int i;

    i = 0;
    while (herdoc)
    {
        herdoc->fd = -1;
        tmp = ft_itoa(i);
        s = str_join("/tmp/",tmp);
        free(tmp);
        if(!herdoc->next)
           herdoc->fd = open(s, O_WRONLY | O_CREAT | O_APPEND, 0640);
        free (s);
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
        herdoc = herdoc->herdoc_next;
        i++;
    }
    exit(0);
}

