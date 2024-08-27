/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exectst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 09:00:40 by haouky            #+#    #+#             */
/*   Updated: 2024/08/27 12:28:01 by haouky           ###   ########.fr       */
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
        if(!input->next && input->type == HERE_DOC)
            fd = open(input->s, O_RDONLY);
        if(fd < 0)
        {
            if(!input->name)
                ft_printf("minishell: : No such file or directory\n");
            else
                ft_printf("minishell: %s: %s\n",input->name, strerror(errno));
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
            fd = open(output->name, O_CREAT | O_TRUNC | O_WRONLY , 0640);
        else if(output->type == DREDIR_OUT)
            fd = open(output->name,  O_CREAT | O_APPEND | O_WRONLY , 0640);
        if(fd < 0)
        {
             if(!output->name)
                ft_printf("minishell: : No such file or directory\n");
            else
                ft_printf("minishell: %s: %s\n",output->name, strerror(errno));
            return (-1);
        }
        output = output->next;
    }
    return (fd);
}

void exccmd(t_execution *exec, t_list *env, int *fd, int old_read)
{
    int infd;
    int outfd;

    infd = in_fd(exec->input, old_read);
    if(infd == -1)
        exit (1);
    outfd = out_fd(exec->output, fd[1],exec->pipe);
    if(outfd == -1)
        exit(1);
    dup2(infd, 0);
    if(infd)   
        close(infd);
    if(fd[0])
        close(fd[0]);
    dup2(outfd, 1);
    if(outfd != 1)
        close(outfd);
    if(exec->cmd)
        infd = check_builtins(exec->cmd[0]);
    if(exec->cmd && infd)
    {
        infd = execute_builtins(&exec->cmd[1], &env, infd, 1);
        exit(infd);   
    }
    if(exec->cmd[0] && !exec->path)
    {   
        if(find_c(exec->cmd[0], '/') || !envv("$PATH",env, 0))
            ft_printf("minishell: %s : No such file or directory\n",exec->cmd[0]);
        else
            ft_printf("minishell: %s : command not found\n",exec->cmd[0]);
        exit(127);
    }
    if(!exec->cmd[0])
        exit(0);
    if (execve(exec->path, exec->cmd, getarray(env)) == -1)
		perror("execve");
    exit(1);
}
int run_execution(t_execution *execution, t_list *env, int status)
{
    t_oip *herdoc;
    int fd[2];
    pid_t *pid;
    int oldread;
    int i;
    int statu;
    int size;
    
    pid = 0;
    i = 0;
    statu = 0;
    herdoc = get_here_doc(execution);
    if(herdoc)
         statu = run_here_doc(herdoc, env, status);
    if(statu)
        return (statu);
    fd[0] = 0;
    fd[1] = 1;
    if(execution->cmd[0])
        i = check_builtins(execution->cmd[0]);
    if(i && !execution->pipe)
    {
        if(in_fd(execution->input, 0) == -1)
            return (1);
        return(execute_builtins(&execution->cmd[1], &env, i, out_fd(execution->output, 1, 0)));
    }
    size = cmd_lst_size(execution);
    pid = malloc(sizeof(pid_t) * size);
    i = 0;
    while (execution)
    {
        oldread = fd[0];
        if(execution->pipe)
        {
            if(pipe(fd) == -1)
            {
                perror("minishel: pipe: ");
                return(1);
            }  
        }
        pid[i] = fork();
        if(pid[i] == -1)
        {
            perror("minishell: fork: ");
            return (1);
        }
        if(pid[i]  == 0)
            exccmd(execution, env, fd, oldread);
        if(oldread)
            close(oldread);
        if(fd[1] != 1)
            close(fd[1]);
        execution = execution->next;
        i++;
    }
    if(fd[0])
        close(fd[0]);
    i = 0;
    while (i < size)
        waitpid(pid[i++], &statu, 0);
     if (WIFEXITED(statu)) 
        statu = WEXITSTATUS(statu);
     else if (WIFSIGNALED(statu))
        statu = WTERMSIG(statu) + 128;
    return (statu);
}

