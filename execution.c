#include "minishell.h"


int	get_in_fd(t_oip *input, int prev_pipe, int *flag)
{
	int	in_fd;

	in_fd = 0;
	if(input)
	{
		in_fd = open_in_files(input);
		
	}
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
		if(!input->next && input->type == HERE_DOC)
		{
			fd = input->fd;
		}
		if(fd == -1)
		{
			ft_write("minishell: ", 2);
			ft_write(input->name, 2);
			ft_write(": ", 2);
			perror("");
			return(-1);
		}
		input = input->next;
	}
	return(fd);
}
int	get_out_fd(t_execution *list, int *prev_pipe, int *flag, int pipe_fd[2])
{
	int	out_fd;

	out_fd = 1;
	if(list->output)
	{
		out_fd = open_out_files(list->output);
		if(out_fd == -1)
		{
			ft_write("minishell: ", 2);
			ft_write(list->output->name, 2);
			ft_write(": ", 2);
			perror("");
			return(-1);
		}
	}
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
			out_fd = pipe_fd[1];
		}
	}
	return(out_fd);
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
			return(fd);
		if(!out_file->next)
			return (fd);
		out_file = out_file->next;
	}
	return(fd);
}

int	check_builtins(char *str)
{
	if(!str_ncomp(str, "echo", 5))
		return(1);
	else if(!str_ncomp(str, "cd", 3))
		return(2);
	else if(!str_ncomp(str, "pwd", 4))
		return(3);
	else if(!str_ncomp(str, "export", 7))
		return(4);
	else if(!str_ncomp(str, "unset", 6))
		return(5);
	else if(!str_ncomp(str, "env", 4))
		return(6);
	else if(!str_ncomp(str, "exit", 5))
		return(7);
	return(0);
}

int	execute_builtins(char **cmd, t_list **env, int flag, int out_fd)
{
	if(flag == 1)
		return (ft_echo(cmd, out_fd));
	else if(flag == 2)
		return (ft_cd(cmd[0]));
	else if(flag == 3)
		return (ft_pwd(out_fd));
	else if(flag == 4)
		return (ft_export(env, cmd[0]));
	else if(flag == 5)
		return (ft_unset(env, cmd[0]));
	else if(flag == 6)
		return (ft_env(*env, out_fd));
	else if(flag == 7)
		return (ft_exit());
	return(0);
}

void 	child_pross(t_execution *list, int	prev_pipe, int in_fd, int out_fd, t_list **env)
{
	int	flag;

	flag = 0;
	printf("in child process in_fd = %d, out_fd == %d, prev_pipe = %d\n", in_fd, out_fd, prev_pipe);
	close (prev_pipe);
	if(in_fd == -1 || out_fd == -1)
		exit(1);
	flag = check_builtins(list->cmd[0]);
	if(flag > 0)
		exit(execute_builtins(&list->cmd[1], env, flag, out_fd));
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
	if(!envv("$PATH",*env, 0))
	{
		ft_write("minishell: ", 2);
		ft_write(list->cmd[0], 2);
		ft_write(": No such file or directory\n", 2);
		exit(127);
	}
	if(!list->path)
	{
		ft_write("minishell: ", 2);
		ft_write(list->cmd[0], 2);
		ft_write(": command not found\n", 2);
		exit(127);
	}
	execve(list->path, list->cmd, getarray(*env));
	error("execve fails\n");
}


int run_cmd(t_execution *list, t_list **env)
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
	int	state;
	int nf;
	t_oip *her_doc;

	her_doc = get_here_doc(list);
	nf = 0;
	flag = 0;
	exit_status = 0;
	prev_pipe = 0;
	exit_status = 0;
	if(her_doc)
	{
		exit_status = run_here_doc(her_doc);
	}		
	if(exit_status)
		return(exit_status);
	size = cmd_lst_size(list);
	pid = malloc(sizeof(pid_t) * size);

	while(list)
	{
		in_fd = get_in_fd(list->input, prev_pipe, &flag);
		out_fd = get_out_fd(list, &prev_pipe, &flag, pipe_fd);
		if(check_builtins(list->cmd[0]) > 0 && size == 1)
		{
			exit_status = execute_builtins(&list->cmd[1], env, check_builtins(list->cmd[0]), out_fd);
			nf = 1;
		}
		else
		{
			pid[i] = fork();
			if (pid[i++] == 0)
				child_pross(list, prev_pipe, in_fd, out_fd, env);
		}
		
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
		waitpid(pid[i++], &state, 0);
	if(nf == 0)
		exit_status = WEXITSTATUS(state);
	return(exit_status);
}
