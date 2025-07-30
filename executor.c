/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:55:04 by segunes           #+#    #+#             */
/*   Updated: 2025/07/30 17:08:12 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_heredoc(const char *delimiter)
{
	int pipefd[2];
	char *line;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);	// yazmayı kapatıyoruz
	return (pipefd[0]); // okuma ucunu geri döndürüyoruz
}

// void executor_structure(t_ast_tree *node, char **envp, int in_pipeline, int *exit_status)
// {
// 	int status;
// 	int second_status;
// 	pid_t pid;
// 	char *cmd;
// 	int pipefd[2];
// 	pid_t pid1;
// 	pid_t pid2;

// 	if (node->type == NODE_COMMAND)
// 	{
// 		if (!in_pipeline && builtin(args_count(&node->args[0]), node->args, envp, NULL) == 0)
// 		{
// 			*exit_status = 0;
// 			return;
// 		}
// 		pid = fork();
// 		if (pid < 0) // ram sistem kaynağı yetmezse hata döner
// 		{
// 			perror("fork");
// 			return;
// 		}
// 		else if (pid == 0) // bu sürecin görevi komutu çalıştırmak //child process
// 		{
// 			t_redir *redir = node->redir_list;
// 			int fd;

// 			signal(SIGINT, SIG_DFL);
// 			signal(SIGQUIT, SIG_DFL);
// 			while (redir)
// 			{
// 				if (redir->type == REDIR_IN)
// 				{
// 					if (!redir->target)
// 					{
// 						fprintf(stderr, "minishell: No input file provided after <\n");
// 						exit(1);
// 					}
// 					fd = open(redir->target, O_RDONLY);
// 					if (fd < 0)
// 					{
// 						perror(redir->target);
// 						exit(1);
// 					}
// 					if (dup2(fd, STDIN_FILENO) == -1)
// 					{
// 						perror("dup2 (in)");
// 						close(fd);
// 						exit(1);
// 					}
// 					close(fd);
// 				}
// 				else if (redir->type == REDIR_OUT)
// 				{
// 					if (!redir->target)
// 					{
// 						fprintf(stderr, "minishell: No input file provided after <\n");
// 						exit(1);
// 					}
// 					fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 					if (fd < 0)
// 					{
// 						perror(redir->target);
// 						exit(1);
// 					}
// 					if (dup2(fd, STDOUT_FILENO) == -1)
// 					{
// 						perror("dup2 (out)");
// 						close(fd);
// 						exit(1);
// 					}
// 					close(fd);
// 				}
// 				else if (redir->type == APPEND)
// 				{
// 					if (!redir->target)
// 					{
// 						fprintf(stderr, "minishell: No input file provided after <\n");
// 						exit(1);
// 					}
// 					fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 					if (fd < 0)
// 					{
// 						perror(redir->target);
// 						exit(1);
// 					}
// 					if (dup2(fd, STDOUT_FILENO) == -1)
// 					{
// 						perror("dup2 (append)");
// 						close(fd);
// 						exit(1);
// 					}
// 					close(fd);
// 				}
// 				// if (redir->type == HEREDOC)
// 				// {
// 				// 	int heredoc_fd = handle_heredoc(redir->target);
// 				// 	if (heredoc_fd == -1)
// 				// 		exit(1);
// 				// 	dup2(heredoc_fd, STDIN_FILENO);
// 				// 	close(heredoc_fd);
// 				// }
// 				else if (redir->type == HEREDOC)
// 				{

// 					int tmp_fd = handle_heredoc(redir->target);
// 					if (tmp_fd == -1)
// 						exit(1);
// 					if (fd != -1)
// 						close(fd); // önceki heredoc’u kapat
// 					fd = tmp_fd;
// 				}
// 				redir = redir->next;
// 			}
// 			if (fd != -1)
// 			{
// 				if (dup2(fd, STDIN_FILENO) == -1)
// 				{
// 					perror("dup2 (final heredoc)");
// 					close(fd);
// 					exit(1);
// 				}
// 				close(fd);
// 			}

// 			if (in_pipeline && builtin(args_count(&node->args[0]), node->args, envp, NULL) == 0)
// 				exit(0);
// 			cmd = find_path(node->args[0]);
// 			if (cmd != NULL)
// 			{
// 				printf("\a");
// 				if (execve(cmd, node->args, envp) == -1)
// 				{
// 					perror("execve");
// 					exit(1);
// 				}
// 			}
// 			else
// 			{
// 				printf("command not found\n");
// 				exit(127);
// 			}
// 		}
// 		else if (pid > 0) // parent process pid aslında çocuk sürecin PIDsi onu beklemek için
// 		{
// 			waitpid(pid, &status, 0);
// 			signal(SIGINT, signal_catch);

// 			if (WIFSIGNALED(status))
// 			{
// 				int sig = WTERMSIG(status);
// 				if (sig == SIGINT)
// 					*exit_status = 130;
// 				else if (sig == SIGQUIT)
// 				{
// 					write(2, "Quit: 3\n", 8);
// 					*exit_status = 131;
// 				}
// 			}
// 			else
// 			{
// 				*exit_status = WEXITSTATUS(status);
// 			}
// 		}
// 	}
// 	else if (node->type == NODE_PIPE)
// 	{ // Neden 2 Fork Açıyoruz? Çünkü: ls -l komutunu çalıştıracak bir süreç (child)/wc -l komutunu çalıştıracak bir başka süreç (başka bir child)
// 		int child_exit_status = 0;
// 		if (pipe(pipefd) == -1) // aradaki pipe oluşturuluyor burada biri stdine diğeri stdouta bağlanıyor
// 		{						// eğer oluşturulamazsa hata
// 			perror("pipe");
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
// 			executor_structure(node->left, envp, 1, &child_exit_status);
// 			exit(child_exit_status);
// 		}
// 		pid2 = fork();
// 		if (pid2 == 0)
// 		{
// 			signal(SIGINT, SIG_DFL);
// 			signal(SIGQUIT, SIG_DFL);
// 			dup2(pipefd[0], STDIN_FILENO);
// 			close(pipefd[1]);
// 			close(pipefd[0]);
// 			executor_structure(node->right, envp, 1, &child_exit_status);
// 			exit(child_exit_status);
// 		}
// 		if (pid1 != 0 && pid2 != 0)
// 		{
// 			close(pipefd[0]);
// 			close(pipefd[1]);
// 			signal(SIGINT, SIG_IGN);
// 			waitpid(pid1, &status, 0);
// 			waitpid(pid2, &second_status, 0);
// 			signal(SIGINT, signal_catch);
// 			// if (exit_status)
// 			// 	*exit_status = WEXITSTATUS(second_status);
// 			if (WIFSIGNALED(second_status))
// 			{
// 				int sig = WTERMSIG(second_status);
// 				if (sig == SIGINT)
// 					*exit_status = 130;
// 				else if (sig == SIGQUIT)
// 				{
// 					write(2, "Quit: 3\n", 8);
// 					*exit_status = 131;
// 				}
// 			}
// 			else if (WIFSIGNALED(status))
// 			{
// 				int sig = WTERMSIG(status);
// 				if (sig == SIGINT)
// 					*exit_status = 130;
// 				else if (sig == SIGQUIT)
// 				{
// 					write(2, "Quit: 3\n", 8);
// 					*exit_status = 131;
// 				}
// 			}
// 			else
// 			{
// 				*exit_status = WEXITSTATUS(second_status);
// 			}
// 		}
// 	}

// else if (node->type == NODE_REDIR)
// {
// 	if (node->redir_type == REDIR_IN || node->redir_type == REDIR_OUT)
// 	{
// 		if (node->redir_type == REDIR_IN)
// 		{
// 			fd = open(node->file, O_RDONLY);
// 			if (fd < 0)
// 			{
// 				perror("open");
// 				// exit çıkışı
// 			}
// 			dup2(fd, STDIN_FILENO); // stdin'i dosyaya yönlendir
// 			close(fd);				// dosya tanımlayıcısını kapat
// 			executor_structure(node->left, envp, in_pipeline, exit_status);
// 		}
// 		else if (node->redir_type == REDIR_OUT)
// 		{
// 			fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 			// O_WRONLY ➝ Sadece yazma O_CREAT ➝ Dosya yoksa oluştur O_TRUNC ➝ Dosya varsa içeriği sıfırla 0644 ➝ Yetkiler (rw-r--r--)
// 			if (fd < 0)
// 			{
// 				perror("open");
// 				// exit çıkışı
// 			}
// 			dup2(fd, STDOUT_FILENO); // stdout'u dosyaya yönlendir
// 			close(fd);				 // dosya tanımlayıcısını kapat
// 			executor_structure(node->left, envp, in_pipeline, exit_status);
// 		}
// 	}
// 	else if (node->redir_type == APPEND || node->redir_type == HEREDOC)
// 	{
// 	}
// }
//}
void executor_structure(t_ast_tree *node, char **envp, int in_pipeline, int *exit_status)
{
	int status, second_status;
	pid_t pid, pid1, pid2;
	int pipefd[2];
	char *cmd;

	if (node->type == NODE_COMMAND)
	{
		// ---- PIPE DIŞINDA BUILTIN ----
		if (!in_pipeline && builtin(args_count(&node->args[0]), node->args, envp, NULL) == 0)
		{
			*exit_status = 0;
			return;
		}

		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			*exit_status = 1;
			return;
		}
		else if (pid == 0) // ---- CHILD ----
		{
			t_redir *redir = node->redir_list;
			int fd_in = -1, fd_out = -1;

			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);

			// ---- REDIRECTIONS ----
			while (redir)
			{
				fprintf(stderr, "[DEBUG] processing redirection type=%d target=%s\n",
						redir->type, redir->target ? redir->target : "(null)");

				// ---- Target Kontrolü ----
				if (!redir->target || redir->target[0] == '\0')
				{
					fprintf(stderr, "minishell: ambiguous redirect\n");
					exit(2); // bash exit code 2
				}

				if (redir->type == REDIR_IN)
				{
					if (fd_in != -1)
						close(fd_in);
					fd_in = open(redir->target, O_RDONLY);
					if (fd_in < 0)
					{
						perror(redir->target);
						exit(1);
					}
					fprintf(stderr, "[DEBUG] opened input fd=%d for %s\n", fd_in, redir->target);
				}
				else if (redir->type == REDIR_OUT)
				{
					if (fd_out != -1)
						close(fd_out);
					fd_out = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (fd_out < 0)
					{
						perror(redir->target);
						exit(1);
					}
					fprintf(stderr, "[DEBUG] opened output fd=%d (truncate) for %s\n", fd_out, redir->target);
				}
				else if (redir->type == APPEND)
				{
					if (fd_out != -1)
						close(fd_out);
					fd_out = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
					if (fd_out < 0)
					{
						perror(redir->target);
						exit(1);
					}
					fprintf(stderr, "[DEBUG] opened output fd=%d (append) for %s\n", fd_out, redir->target);
				}
				else if (redir->type == HEREDOC)
				{
					int tmp_fd = handle_heredoc(redir->target);
					if (tmp_fd == -1)
						exit(1);
					if (fd_in != -1)
						close(fd_in);
					fd_in = tmp_fd;
					fprintf(stderr, "[DEBUG] heredoc fd=%d\n", fd_in);
				}
				redir = redir->next;
			}

			// ---- FD DUP ----
			if (fd_in != -1)
			{
				if (dup2(fd_in, STDIN_FILENO) == -1)
				{
					perror("dup2 in");
					exit(1);
				}
				close(fd_in);
			}
			if (fd_out != -1)
			{
				if (dup2(fd_out, STDOUT_FILENO) == -1)
				{
					perror("dup2 out");
					exit(1);
				}
				close(fd_out);
			}

			// ---- PIPE İÇİN BUILTIN ----
			if (in_pipeline && builtin(args_count(&node->args[0]), node->args, envp, NULL) == 0)
				exit(0);

			// ---- EXECVE ----
			cmd = find_path(node->args[0]);
			if (cmd)
			{
				execve(cmd, node->args, envp);
				perror("execve");
				exit(1);
			}
			fprintf(stderr, "%s: command not found\n", node->args[0]);
			exit(127);
		}
		else // ---- PARENT ----
		{
			waitpid(pid, &status, 0);
			signal(SIGINT, signal_catch);

			if (WIFSIGNALED(status))
			{
				int sig = WTERMSIG(status);
				*exit_status = (sig == SIGINT) ? 130 : (sig == SIGQUIT ? 131 : 1);
				if (sig == SIGQUIT)
					write(2, "Quit: 3\n", 8);
			}
			else
				*exit_status = WEXITSTATUS(status);
		}
	}
	else if (node->type == NODE_PIPE)
	{
		int child_exit_status = 0;
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			*exit_status = 1;
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
			executor_structure(node->left, envp, 1, &child_exit_status);
			exit(child_exit_status);
		}

		pid2 = fork();
		if (pid2 == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[1]);
			close(pipefd[0]);
			executor_structure(node->right, envp, 1, &child_exit_status);
			exit(child_exit_status);
		}

		close(pipefd[0]);
		close(pipefd[1]);
		signal(SIGINT, SIG_IGN);
		waitpid(pid1, &status, 0);
		waitpid(pid2, &second_status, 0);
		signal(SIGINT, signal_catch);

		if (WIFSIGNALED(second_status))
		{
			int sig = WTERMSIG(second_status);
			*exit_status = (sig == SIGINT) ? 130 : (sig == SIGQUIT ? 131 : 1);
			if (sig == SIGQUIT)
				write(2, "Quit: 3\n", 8);
		}
		else
			*exit_status = WEXITSTATUS(second_status);
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