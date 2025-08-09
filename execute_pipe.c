//header

#include "minishell.h"

static void handle_pipe_status(int status)
{
	int sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			ft_exit_code(130);
		else if (sig == SIGQUIT)
		{
			write(2, "Quit: 3\n", 8);
			ft_exit_code(131);
		}
		else if (sig == SIGPIPE)
		{
			write(2, "Broken pipe\n", 12);
			ft_exit_code(141);
		}
	}
	else
		ft_exit_code(WEXITSTATUS(status));
}

static pid_t	spawn_pipe_child(t_ast_tree *node, char ***envp, int pipefd[2], char* side)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (ft_strcmp(side, "left") == 0)
		{
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			executor_structure(node->left, envp, 1);
		}
		else
		{
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[1]);
			close(pipefd[0]);
			executor_structure(node->right, envp, 1);
		}
		exit(ft_exit_code(-1));
	}
	return pid;
}

void	execute_pipe(t_ast_tree *node, char ***envp)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		ft_exit_code(1);
		return;
	}
	pid1 = spawn_pipe_child(node, envp, pipefd, "left");
	pid2 = spawn_pipe_child(node, envp, pipefd, "right");
	close(pipefd[0]);
	close(pipefd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	signal(SIGINT, signal_catch);
	if ((WIFSIGNALED(status1) && WTERMSIG(status1) == SIGPIPE)
	 || (WIFEXITED(status1) && WEXITSTATUS(status1) == 141))
		write(2, " Broken pipe\n", 12);
	else
		handle_pipe_status(status2);
}
