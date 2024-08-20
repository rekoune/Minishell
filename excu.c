#include "minishell.h"

int	open_in_files(t_oip *input)
{
	int fd;

	fd = 0;
	while(input)
	{
		if (fd)
			close(fd);
		if(input->type == REDIR_IN)
			fd = open(input->name, O_RDWR);
		//here print in the standatd error
		if(fd == -1)
			error("ERROR: No such file or directory\n");
		if(!input->next)
			break;
		input = input->next;
	}
	return(fd);
}

int	open_out_files(t_oip *out_file)
{
	int fd;

	fd = 1;
	while(out_file)
	{
		if(out_file->type == REDIR_OUT)
			fd = open(out_file->name, O_CREAT | O_TRUNC | O_WRONLY ,0640);
		else if (out_file->type == DREDIR_OUT)
			fd = open(out_file->name, O_CREAT | O_APPEND | O_WRONLY ,0640);
		if(fd == -1)
			error("ERROR: No such file or directory\n");
		if(!out_file->next)
			return (fd);
		out_file = out_file->next;
	}
	return(fd);
}

void 	child_pross(t_excution *list, int in_fd, int out_fd, char **env)
{
	if (in_fd != 0)
	{
		dup2(in_fd, 0);
		close(in_fd);
	}
	if (out_fd != 1)
	{
		dup2(out_fd, 1);
		close(out_fd);
	}
	execve(list->path, list->cmd, env);
	printf("execve fails\n");
}

int	get_in_fd(t_oip *input, int prev_pipe, int *flag)
{
	int	in_fd;

	in_fd = 0;
	if(input)
		in_fd = open_in_files(input);
	else if(*flag == 1)
	{
		in_fd = dup(prev_pipe);
		close(prev_pipe);
		*flag = 0;
	}
	else
		in_fd = 0;
	return(in_fd);
}

int	get_out_fd(t_excution *list, int *prev_pipe, int *flag, int pipe_fd[2])
{
	int	out_fd;

	out_fd = 1;
	if(list->output)
			out_fd = open_out_files(list->output);
		else 
			out_fd = 1;
		if(list->pipe == 1)
		{
			if(pipe(pipe_fd) == -1)
				error("ERROR : pipe fails\n");
			*prev_pipe = dup(pipe_fd[0]);
			close(pipe_fd[0]);
			*flag = 1;
			if(list->output == NULL)
			{
				out_fd = dup(pipe_fd[1]);
				close(pipe_fd[1]);
			}
		}
	return(out_fd);
}

// void	ft_close(t_excution *list, int in_fd, int out_fd, char **env)
// {
	
// }

void	run_cmd(t_excution *list, char **env)
{
	int	pipe_fd[2];
	int	prev_pipe;
	int	in_fd;
	int	out_fd;
	int	flag;
	pid_t *pid;
	int	exit_status;
	int	i = 0;
	int	size;

	flag = 0;
	prev_pipe = 0;
	exit_status = 0;
	size = cmd_lst_size(list);
	pid = malloc(sizeof(pid_t) * size);
	while(list)
	{
		in_fd = get_in_fd(list->input, prev_pipe, &flag);
		out_fd = get_out_fd(list, &prev_pipe, &flag, pipe_fd);
		pid[i] = fork();
		if (pid[i++] == 0)
			child_pross(list, in_fd, out_fd, env);
		if(in_fd != 0)
			close(in_fd);
		if(out_fd != 1)
			close(out_fd);
		if(flag == 1 && list->output)
			close(pipe_fd[1]);
		list = list->next;
	}
	i = 0;
	while(i < size)
		waitpid(pid[i++], NULL, 0);
}
