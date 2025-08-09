/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:55:04 by segunes           #+#    #+#             */
/*   Updated: 2025/08/07 10:15:25 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void handle_command_status(int status)
{
	if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		if (sig == SIGINT)
			ft_exit_code(130);
		else if (sig == SIGQUIT)
		{
			write(2, "Quit: 3\n", 8);
			ft_exit_code(131);
		}
		else if (sig == SIGPIPE)
            ft_exit_code(141);
	}
	else
		ft_exit_code(WEXITSTATUS(status));
}

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

static void execute_pipe(t_ast_tree *node, char ***envp)
{
	int pipefd[2];
	pid_t pid1;
	pid_t pid2;
	int status1;
	int status2;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		ft_exit_code(1);
		return;
	}
	pid1 = fork();
	if (pid1 == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		executor_structure(node->left, envp, 1);
		exit(ft_exit_code(-1));
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
		executor_structure(node->right, envp, 1);
		exit(ft_exit_code(-1));
	}
	close(pipefd[0]);
	close(pipefd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	signal(SIGINT, signal_catch);
	if ( (WIFSIGNALED(status1) && WTERMSIG(status1) == SIGPIPE) ||
         (WIFEXITED(status1)  && WEXITSTATUS(status1) == 141) )
        write(2, " Broken pipe\n", 12);
	else
		handle_pipe_status(status2);
}

void executor_structure(t_ast_tree *node, char ***envp, int in_pipeline)
{
	// Temel kontroller
	int std_in;
	int std_out;

	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);

	if (node->type == NODE_COMMAND)
	{
		// Pipe dışındaki builtin komutları direkt çalıştır
		if (!in_pipeline && (is_builtin(node->args[0])))
		{
			if (handle_redirections(node) != 0)
			{
				ft_exit_code(1);
				dup2(std_in, STDIN_FILENO);	  // input'u geri al
				dup2(std_out, STDOUT_FILENO); // output'u geri al
				return;
			}

			int argc = args_count(node->args);
			int status = builtin(argc, node->args, envp);

			dup2(std_in, STDIN_FILENO);
			dup2(std_out, STDOUT_FILENO);

			if (status != -1)
			{
				ft_exit_code(status);
				return;
			}
		}
		// Harici komutlar için fork
		pid_t pid = fork();
		if (pid < 0)
		{
			perror("fork");
			ft_exit_code(1);
			return;
		}
		else if (pid == 0) // Child process
		{
			// Child process'te execute_command çağır
			execute_command(node, envp, in_pipeline);
			// Bu noktaya asla ulaşılmamalı, execute_command her zaman exit() çağırır
			exit(1);
		}
		else // Parent process
		{
			int status;
			// Child process'i bekle
			signal(SIGINT, signal_child);
			waitpid(pid, &status, 0);
			// Exit status'u işle
			handle_command_status(status);
			signal(SIGINT, signal_catch);

			// Yönlendirmeleri temizle
			if (node->redir_list)
			{
				t_redir *current = node->redir_list;
				t_redir *next;
				while (current)
				{
					next = current->next;
					if (current->target)
						free(current->target);
					free(current);
					current = next;
				}
				node->redir_list = NULL;
			}
		}
	}
	else if (node->type == NODE_PIPE)
		execute_pipe(node, envp);
}

/*
| pid == 0 | Bu kod bloğu child process içindir |
| pid > 0 | Bu kod bloğu parent process içindir |
| pid < 0 | Hata: fork başarısız oldu |

********************************************************************

dup() sistem çağrısı bir dosya tanımlayıcısının kopyasını oluşturur.

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
															 !!
			  [ PARENT PROCESS ]                             !!
					 |                                       !!
				pipe(pipefd)                                 !!
					 |                                       !!
			  ┌──────┴──────┐                                !!
			  |             |                                !!
		fork() →         fork()                              !!
		 |                 |                                 !!
	 [ CHILD 1 ]       [ CHILD 2 ]                           !!
	  echo serap        wc -c                                !!
	 stdout → pipe      stdin ← pipe                         !!
		 |                  ^                                !!
		 └──── pipefd[1]    └──── pipefd[0]                  !!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

*/