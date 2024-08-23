/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exectst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 09:00:40 by haouky            #+#    #+#             */
/*   Updated: 2024/08/23 16:53:19 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int in_fd(t_oip *input, int fd)
{
    while (input)
    {
        if(fd)
            close(fd);
        if(input->type == REDIR_IN)
            fd = open(input->name, O_RDONLY);
        if(fd < 0)
        {
            printf("minishell: no such file or directory : %s\n", input->name);
            return (-1); 
        }
        input = input->next;
    }
    return (fd);
}

int out_fd(t_oip *output,int fd, int pipe)
{
    if(!output && !pipe)
        return (1); 
    while (output)
    {
        if(fd != 1)
            close(fd);
        if(output->type == REDIR_OUT)
            fd = open(0, O_WRONLY | O_CREAT | O_APPEND, 0640);
        else if(output->type == DREDIR_OUT)
            fd = open(0, O_WRONLY | O_CREAT | O_TRUNC, 0640);
        if(fd < 0)
        {
            perror("minishell:");
            return (-1);
        }
        output = output->next;
    }
    return (fd);
}

void exccmd(t_excution *exec, t_list *env, int *fd, int old_read)
{
    int infd;
    int outfd;

    if(!exec->path)
    {   
        printf("minishell: %s: command not found\n",exec->cmd[0]);
        exit(127);
    }
    infd = in_fd(exec->input, old_read);
    if(infd == -1)
        exit (1);
    outfd = out_fd(exec->output, fd[1],exec->pipe);
    if(outfd == -1)
        exit(1);
    dup2(infd, 0);
    dprintf(2,"infd: %d\n",infd);
    close(infd);
    if(fd[0])
        close(fd[0]);
    dup2(outfd, 1);
    dprintf(2,"outfd: %d\n",outfd);
    close(outfd);
    if (execve(exec->path, exec->cmd, getarray(env)) == -1)
		perror("execve");
    exit(1);
}
int run_exuction(t_excution *exuction, t_list *env)
{
    t_oip *herdoc;
    int fd[2];
    int pid;
    int oldread;
    int i;
    
    i = 0;
    herdoc = get_here_doc(exuction);
    // if(herdoc)
    //     run_here_doc(herdoc);
    if(exuction->pipe)
    { 
        if(pipe(fd) == -1)
        {
            perror("minishel:");
            return(1);
        }
    }
    fd[0] = 0;
    fd[1] = 1;
    while (exuction)
    {
        oldread = fd[0];
        if(pipe(fd) == -1)
        {
            perror("minishel:");
            return(1);
        }
        pid = fork();
        if(pid == -1)
        {
            perror("minishell:");
            return (1);
        }
        if(pid == 0)
            exccmd(exuction, env, fd, oldread);
        if(oldread)
            close(oldread);
        close(fd[1]);
        exuction = exuction->next;
        i++;
    }
    close(fd[0]);
    while (wait(&pid) && i--)
    ;
    return (pid);
}

