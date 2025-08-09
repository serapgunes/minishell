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