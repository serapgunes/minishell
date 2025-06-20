/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:55:04 by segunes           #+#    #+#             */
/*   Updated: 2025/06/20 16:32:45 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void executor_structure(t_ast_tree *node, char **envp)
{
	pid_t pid;
	char *cmd;
	int pipefd[2];
	pid_t pid1;
	pid_t pid2;

	if(node->type == NODE_COMMAND)
	{
		if(builtin(args_count(&node->args[0]),node->args, envp, NULL) == 0)
			return;
		pid = fork();
		if(pid < 0)//ram , sistem kaynağı yetmezse hata döner
		{
			perror("fork");
			return;
		}
		else if(pid == 0)//bu sürecin görevi komutu çalıştırmak //child process
		{
			cmd = find_path(node->args[0]);
			if(cmd != NULL)
			{
				if(execve(cmd, node->args, envp) == -1)
				{
					perror("execve");
					exit(1);
				}
			}
			else
			{
				printf("command not found\n");
				exit(127);
			}
		}
		else if(pid > 0)//parent process pid aslında çocuk sürecin PIDsi onu beklemek için
			wait(NULL);
	}
	else if(node->type == PIPE)
	{//Neden 2 Fork Açıyoruz? Çünkü: ls -l komutunu çalıştıracak bir süreç (child)/wc -l komutunu çalıştıracak bir başka süreç (başka bir child)

		if(pipe(pipefd)== -1)//aradaki pipe oluşturuluyor burada biri stdine diğeri stdouta bağlanıyor
		{//eğer oluşturulamazsa hata
			perror("pipe");
			return;
		}
		pid1 = fork();
		if(pid1 == 0)
		{
			dup2(pipefd[1], STDOUT_FILENO);//file descriptor kopyalayan sistem çağrısı
			//dup2 pipefd nin içeriğini tam olarak stdouta yönlendirir daha sonra pipefd deki stdouta kopyalanır
			close(pipefd[0]);
			close(pipefd[1]);
			executor_structure(node->left,envp);
			exit(0);
		} 
		pid2 = fork();
		if(pid2 == 0)
		{
			dup2(pipefd[0],STDIN_FILENO);
			close(pipefd[1]);
			close(pipefd[0]);
			executor_structure(node->right,envp);
			exit(0);
		}
		if(pid1 != 0 && pid2 != 0)
		{
			close(pipefd[0]);
			close(pipefd[1]);
			waitpid(pid1, NULL, 0);
			waitpid(pid2, NULL, 0);
		}

	}
	else if(node->type == REDIR_IN || node->type == REDIR_OUT)
	{

	}
	else if(node->type == APPEND || node->type == HEREDOC)
	{
		
	}
}

/*
| pid == 0 | Bu kod bloğu child process içindir |
| pid > 0 | Bu kod bloğu parent process içindir |
| pid < 0 | Hata: fork başarısız oldu |

********************************************************************

dup() sistem çağrısı bir dosya tanımlayıcısının kopyasını oluşturur.

*/
