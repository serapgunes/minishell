/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:55:04 by segunes           #+#    #+#             */
/*   Updated: 2025/06/17 17:49:42 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void executor_structure(t_ast_tree *node, char **envp)
{
	pid_t pid;
	char *cmd;

	if(node->type == NODE_COMMAND)
	{
		if(builtin(args_count(&node->args[0]),node->args, envp, NULL) == 0)
			return;
		pid = fork();
		if(pid == 0)
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
		else if(pid > 0)
			wait(NULL);
	}
	else if(node->type == PIPE)
	{

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
*/