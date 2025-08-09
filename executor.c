/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:55:04 by segunes           #+#    #+#             */
/*   Updated: 2025/08/10 02:38:42 by segunes          ###   ########.fr       */
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

static int try_execute_builtin(t_ast_tree *node, int in_pipeline, int std_in, int std_out, t_shell *shell)
{
	int argc;
	int status;

	if (!in_pipeline && is_builtin(node->args[0]))
	{
		if (handle_redirections(node) != 0)
		{
			ft_exit_code(1);
			dup2(std_in, STDIN_FILENO);	  // input'u geri al
			dup2(std_out, STDOUT_FILENO); // output'u geri al
			close(std_in);
			close(std_out);
			return (1);
		}
		argc = args_count(node->args);
		status = builtin(argc, node->args, &shell->envp);
		dup2(std_in, STDIN_FILENO);
		dup2(std_out, STDOUT_FILENO);
		close(std_in);
		close(std_out);
		if (status != -1)
		{
			ft_exit_code(status);
			return (1);
		}
	}
	return (0);
}

static void execute_and_wait(t_ast_tree *node, int in_pipeline, t_shell *shell)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		ft_exit_code(1);
		return;
	}
	else if (pid == 0) // Child process
	{
		execute_command(node, in_pipeline, shell); // Child process'te execute_command çağır
		exit(1);								   // Bu noktaya asla ulaşılmamalı, execute_command her zaman exit() çağırır
	}
	else // Parent process
	{
		signal(SIGINT, signal_child); // Child process'i bekle
		waitpid(pid, &status, 0);
		handle_command_status(status); // Exit status'u işle
		signal(SIGINT, signal_catch);
	}
}

static void free_redirections(t_ast_tree *node)
{
	t_redir *current;
	t_redir *next;

	current = node->redir_list;
	while (current)
	{
		next = current->next;
		if (current->type == HEREDOC && current->fd >= 0)
			close(current->fd);
		if (current->target)
			free(current->target);
		free(current);
		current = next;
	}
	node->redir_list = NULL;
}

void executor_structure(t_ast_tree *node, int in_pipeline, t_shell *shell)
{
	int std_in;
	int std_out;

	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	if (node->type == NODE_COMMAND)
	{
		if (try_execute_builtin(node, in_pipeline, std_in, std_out, shell))
			return;
		execute_and_wait(node, in_pipeline, shell);
		if (node->redir_list)
			free_redirections(node);
	}
	else if (node->type == NODE_PIPE)
		execute_pipe(node, shell);
	close(std_in);
	close(std_out);
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