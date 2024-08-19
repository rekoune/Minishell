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

void	run_cmd(t_excution *list, char **env)
{
	int	pipe_fd[2];
	int	prev_pipe = 0;
	int	in_fd;
	int	out_fd;
	int	flag;
	pid_t pid;

	in_fd = 0;
	out_fd = 1;
	flag = 0;
	while(list)
	{
		if(list->input)
			in_fd = open_in_files(list->input);
		else if(flag == 1)
		{
			in_fd = dup(prev_pipe);
			close(prev_pipe);
			flag = 0;
		}
		else
			in_fd = 0;
		if(list->pipe == 1)
		{
			if(pipe(pipe_fd) == -1)
				error("ERROR : pipe fails\n");
			prev_pipe = dup(pipe_fd[0]);
			flag = 1;
		}
		if(list->output)
			out_fd = open_out_files(list->output);
		else if(list->pipe == 1 && list->output == NULL)
		{
			out_fd = dup(pipe_fd[1]);
			close(prev_pipe);
			prev_pipe = dup(pipe_fd[0]);
			close(pipe_fd[1]);
			close(pipe_fd[0]);
			flag = 1;
		}
		else 
			out_fd = 1;
		pid = fork();
		if (pid == 0)
		{
			child_pross(list, in_fd, out_fd, env);
		}
		waitpid(pid, NULL, 0);
		if(in_fd != 0)
			close(in_fd);
		if(out_fd != 1)
			close(out_fd);
		if(flag == 1)
		{
			close(pipe_fd[1]);
			close(pipe_fd[0]);
		}
		list = list->next;
	}

}