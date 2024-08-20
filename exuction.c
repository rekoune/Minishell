/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exuction.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:48:20 by haouky            #+#    #+#             */
/*   Updated: 2024/08/20 09:25:53 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


t_oip  *get_here_doc(t_excution *exuction)
{
    t_oip *inp;
    t_oip *sinp;
    t_oip *firstehrdoc;

    sinp = 0;
    firstehrdoc = 0;
    while (exuction)
    {
        inp = exuction->input;
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
        exuction = exuction->next;
    }
    return (firstehrdoc);
}

void run_here_doc(t_oip *herdoc)
{
    char *tmp;
    char *s;
    char *dlm;

    printf("her\n");
    while (herdoc)
    {
        herdoc->s = 0;
        dlm = str_join(herdoc->name, "\n");
        write(1,">",1);
        s = get_next_line(0);
        printf("s =%s&&dlm =%s\n",s , dlm);
        while (s)
        {
            if(str_comp(s, dlm))
            {
                printf ("i'm breaking\n");
                break;
            }
            tmp = herdoc->s;
            herdoc->s = str_join(herdoc->s, s);
            free(tmp);
            free(s);
            write(1,">",1);
            s = get_next_line(0);
        } 
        free(dlm);      
        herdoc = herdoc->herdoc_next;
    }
    exit (0); 
}

void run_exuction(t_excution *exuction, t_list *env)
{
    t_oip *herdoc;
    int pid;

    env = 0;
    herdoc = get_here_doc(exuction);
    printf("getid the herdoc\n");
    if(herdoc)
    {
        printf("herdoc -> %s\n",herdoc->name);
        pid = fork();
        if(!pid)
            run_here_doc(herdoc);
        printf("i'm whating\n");
        wait(&g_status);
    }
    // printf("herdoc->str = %s\n",herdoc->s);
}
