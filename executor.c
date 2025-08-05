/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:55:04 by segunes           #+#    #+#             */
/*   Updated: 2025/08/05 14:51:10 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// int handle_heredoc(const char *delimiter)
// {
// 	int pipefd[2];
// 	char *line;

// 	if (pipe(pipefd) == -1)
// 	{
// 		perror("pipe");
// 		return (-1);
// 	}
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
// 		{
// 			free(line);
// 			break;
// 		}
// 		write(pipefd[1], line, ft_strlen(line));
// 		write(pipefd[1], "\n", 1);
// 		free(line);
// 	}
// 	close(pipefd[1]);	// yazmayı kapatıyoruz
// 	return (pipefd[0]); // okuma ucunu geri döndürüyoruz
// }

// void executor_structure(t_ast_tree *node, char **envp, int in_pipeline)
// {
// 	int status;
// 	int second_status;
// 	pid_t pid;
// 	pid_t pid1;
// 	pid_t pid2;
// 	int pipefd[2];
// 	char *cmd;

// 	if (node->type == NODE_COMMAND)
// 	{
// 		// ---- PIPE DIŞINDA BUILTIN ----
// 		if (!in_pipeline && builtin(args_count(&node->args[0]), node->args, envp, NULL) == 0)
// 		{
// 			ft_exit_code(0);
// 			return;
// 		}
// 		pid = fork();
// 		if (pid < 0) // ram sistem kaynağı yetmezse hata döner
// 		{
// 			perror("fork");
// 			ft_exit_code(1);
// 			return;
// 		}
// 		else if (pid == 0) // bu sürecin görevi komutu çalıştırmak //child process
// 		{
// 			t_redir *redir = node->redir_list;
// 			int fd_in = -1;
// 			int fd_out = -1;

// 			signal(SIGINT, SIG_DFL);
// 			signal(SIGQUIT, SIG_DFL);
// 			while (redir)
// 			{
// 				// ---- Target Kontrolü ----
// 				if (!redir->target || redir->target[0] == '\0')
// 				{
// 					fprintf(stderr, "minishell: ambiguous redirect\n");
// 					exit(2);
// 				}
// 				if (redir->type == REDIR_IN)
// 				{
// 					if (fd_in != -1)
// 						close(fd_in);
// 					fd_in = open(redir->target, O_RDONLY);
// 					if (fd_in < 0)
// 					{
// 						perror(redir->target);
// 						exit(1);
// 					}
// 				}
// 				else if (redir->type == REDIR_OUT)
// 				{
// 					if (fd_out != -1)
// 						close(fd_out);
// 					fd_out = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 					if (fd_out < 0)
// 					{
// 						perror(redir->target);
// 						exit(1);
// 					}
// 				}
// 				else if (redir->type == APPEND)
// 				{
// 					if (fd_out != -1)
// 						close(fd_out);
// 					fd_out = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 					if (fd_out < 0)
// 					{
// 						perror(redir->target);
// 						exit(1);
// 					}
// 				}
// 				else if (redir->type == HEREDOC)
// 				{
// 					int tmp_fd = handle_heredoc(redir->target);
// 					if (tmp_fd == -1)
// 					{
// 						ft_exit_code(1);
// 						exit(1);
// 					}
// 					if (fd_in != -1)
// 						close(fd_in);
// 					fd_in = tmp_fd;
// 				}
// 				redir = redir->next;
// 			}
// 			if (fd_in != -1)
// 			{
// 				if (dup2(fd_in, STDIN_FILENO) == -1)
// 				{
// 					perror("dup2 in");
// 					exit(1);
// 				}
// 				close(fd_in);
// 			}
// 			if (fd_out != -1)
// 			{
// 				if (dup2(fd_out, STDOUT_FILENO) == -1)
// 				{
// 					perror("dup2 out");
// 					exit(1);
// 				}
// 				close(fd_out);
// 			}
// 			if (in_pipeline && builtin(args_count(&node->args[0]), node->args, envp, NULL) == 0)
// 				exit(0);
// 			cmd = find_path(node->args[0]);
// 			if (cmd)
// 			{
// 				execve(cmd, node->args, envp);
// 				perror("execve");
// 				exit(1);
// 			}
// 			fprintf(stderr, "%s: command not found\n", node->args[0]);
// 			exit(127);
// 		}
// 		else
// 		{
// 			waitpid(pid, &status, 0);
// 			signal(SIGINT, signal_catch);

// 			if (WIFSIGNALED(status))
// 			{
// 				int sig = WTERMSIG(status);
// 				if (sig == SIGINT)
// 					ft_exit_code(130);
// 				if (sig == SIGQUIT)
// 				{
// 					write(2, "Quit: 3\n", 8);
// 					ft_exit_code(131);
// 				}
// 			}
// 			else
// 				ft_exit_code(WEXITSTATUS(status));
// 		}
// 	}
// 	else if (node->type == NODE_PIPE)
// 	{ // Neden 2 Fork Açıyoruz? Çünkü: ls -l komutunu çalıştıracak bir süreç (child)/wc -l komutunu çalıştıracak bir başka süreç (başka bir child)
// 		// int child_exit_status = 0;
// 		if (pipe(pipefd) == -1) // aradaki pipe oluşturuluyor burada biri stdine diğeri stdouta bağlanıyor
// 								// eğer oluşturulamazsa hata
// 		{
// 			perror("pipe");
// 			ft_exit_code(1);
// 			return;
// 		}
// 		pid1 = fork(); // her fork yeni bir çocuk oluşturuyor mesela ls | wc -l ilk çocuk ls ikincisi wc -l
// 		if (pid1 == 0)
// 		{
// 			signal(SIGINT, SIG_DFL);
// 			signal(SIGQUIT, SIG_DFL);
// 			dup2(pipefd[1], STDOUT_FILENO); // file descriptor kopyalayan sistem çağrısı
// 			// dup2 pipefd nin içeriğini tam olarak stdouta yönlendirir daha sonra pipefd deki stdouta kopyalanır
// 			close(pipefd[0]);
// 			// Her çocuk sadece kullandığı ucu açık bırakır.
// 			// Kullanmıyorsa kapatır (close()), yoksa pipe boşu boşuna açık kalır, process’ler bekleyip durur.
// 			close(pipefd[1]);
// 			executor_structure(node->left, envp, 1);
// 			exit(ft_exit_code(-1));
// 		}
// 		pid2 = fork();
// 		if (pid2 == 0)
// 		{
// 			signal(SIGINT, SIG_DFL);
// 			signal(SIGQUIT, SIG_DFL);
// 			dup2(pipefd[0], STDIN_FILENO);
// 			close(pipefd[1]);
// 			close(pipefd[0]);
// 			executor_structure(node->right, envp, 1);
// 			ft_exit_code(ft_exit_code(-1));
// 		}
// 		close(pipefd[0]);
// 		close(pipefd[1]);
// 		signal(SIGINT, SIG_IGN);
// 		waitpid(pid1, &status, 0);
// 		waitpid(pid2, &second_status, 0);
// 		signal(SIGINT, signal_catch);

// 		if (WIFSIGNALED(second_status))
// 		{
// 			int sig = WTERMSIG(second_status);
// 			if (sig == SIGINT)
// 				ft_exit_code(130);
// 			if (sig == SIGQUIT)
// 			{
// 				write(2, "Quit: 3\n", 8);
// 				ft_exit_code(131);
// 			}
// 		}
// 		else
// 			ft_exit_code(WEXITSTATUS(second_status));
// 	}
// }
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

// int handle_heredoc(const char *delimiter)
// {
// 	int pipefd[2];
// 	char *line;
// 	// pipefd[0]=okuma ucu
// 	// pipefed[1]=yazma ucu
// 	if (pipe(pipefd) == -1)
// 	{
// 		perror("pipe");
// 		return -1;
// 	}
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
// 		{
// 			free(line);
// 			break;
// 		}
// 		write(pipefd[1], line, ft_strlen(line));
// 		write(pipefd[1], "\n", 1);
// 		free(line);
// 	}
// 	close(pipefd[1]);
// 	return pipefd[0];
// }

static int handle_redirections(t_ast_tree *node)
{
	t_redir *redir;
	int fd_in;
	int fd_out;

	redir = node->redir_list;
	fd_in = -1;
	fd_out = -1;
	while (redir)
	{
		if (!redir->target || redir->target[0] == '\0')
		{
			write(2, "ambiguous redirect\n", 19);
			return (1);
		}

		if (redir->type == REDIR_IN)
		{
			if (fd_in != -1)
				close(fd_in);
			fd_in = open(redir->target, O_RDONLY);
			if (fd_in < 0)
			{
				perror(redir->target);
				return (1);
			}
		}
		else if (redir->type == REDIR_OUT)
		{
			if (fd_out != -1)
				close(fd_out);
			fd_out = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd_out < 0)
			{
				perror(redir->target);
				return (1);
			}
		}
		else if (redir->type == APPEND)
		{
			if (fd_out != -1)
				close(fd_out);
			fd_out = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd_out < 0)
			{
				perror(redir->target);
				return (1);
			}
		}
		else if (redir->type == HEREDOC)
		{
			if (redir->fd < 0)
			{
				write(2, "invalid heredoc\n", 17);
				return (1);
			}
			if (fd_in != -1)
			{

				close(fd_in);
			}
			fd_in = redir->fd; // 🔄 önce fd'yi al
							   // redir->fd'yi burada kapatma!
		}
		redir = redir->next;
	}
	if (fd_in != -1)
	{
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			perror("dup21");
			close(fd_in);
			return (1);
		}
		close(fd_in);
	}
	if (fd_out != -1)
	{
		if (dup2(fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup22");
			close(fd_out);
			return (1);
		}
		close(fd_out);
	}
	return (0);
}

static void execute_command(t_ast_tree *node, char **envp, int in_pipeline)
{
	int cmd_idx;
	char *cmd;
	char *path;
	int ret;
	struct stat sb;

	(void)in_pipeline;
	cmd_idx = 0;
	if (!node || !node->args)
		exit(1);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	while (node->args[cmd_idx] && node->args[cmd_idx][0] == '\0')
		cmd_idx++;
	if (!node->args[cmd_idx])
	{
		if (cmd_idx == 0)
			exit(1);
		exit(0);
	}
	cmd = node->args[cmd_idx];
	if (handle_redirections(node) != 0)
		exit(1);
	ret = builtin(args_count(node->args + cmd_idx), node->args + cmd_idx, envp, NULL);
	if (ret != -1)
	{
		signal(SIGPIPE, SIG_IGN);
		exit(ret);
	}
	signal(SIGPIPE, SIG_DFL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (stat(cmd, &sb) == -1)
		{
			if (errno == ENOENT)
			{
				write(2, cmd, ft_strlen(cmd));
				write(2, ": No such file or directory\n", 29);
			}
			else
				perror(cmd);
			exit(127);
		}
		if (S_ISDIR(sb.st_mode))
		{
			write(2, cmd, ft_strlen(cmd));
			write(2, ": Is a directory\n", 18);
			exit(126);
		}
		if (access(cmd, X_OK) != 0)
		{
			write(2, cmd, ft_strlen(cmd));
			write(2, ": Permission denied\n", 21);
			exit(126);
		}
		execve(cmd, node->args + cmd_idx, envp);
		perror("execve");
		exit(1);
	}
	else
	{
		path = find_path(cmd);
		if (path)
		{
			execve(path, node->args + cmd_idx, envp);
			free(path);
			perror("execve");
			exit(1);
		}
		write(2, cmd, ft_strlen(cmd));
		write(2, ": command not found\n", 21);
		exit(127);
	}
}

static void execute_pipe(t_ast_tree *node, char **envp)
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
	handle_pipe_status(status2);
}

void executor_structure(t_ast_tree *node, char **envp, int in_pipeline)
{
	// Temel kontroller
	int std_in;
	int std_out;

	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);

	if (node->type == NODE_COMMAND)
	{
		// Pipe dışındaki builtin komutları direkt çalıştır
		if (!in_pipeline && is_builtin(node->args[0]))
		{
			if (handle_redirections(node) != 0)
			{
				ft_exit_code(1);
				dup2(std_in, STDIN_FILENO);	  // input'u geri al
				dup2(std_out, STDOUT_FILENO); // output'u geri al
				return;
			}

			int argc = args_count(node->args);
			int status = builtin(argc, node->args, envp, NULL);

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
			waitpid(pid, &status, 0);
			// Exit status'u işle
			handle_command_status(status);

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
	{
		execute_pipe(node, envp);

		// // Pipe node'larını temizle
		// if (node->left)
		// {
		// 	// Sol tarafı temizle
		// 	executor_structure(node->left, envp, 1);
		// }
		// if (node->right)
		// {
		// 	// Sağ tarafı temizle
		// 	executor_structure(node->right, envp, 1);
		// }
	}
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