/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 22:08:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/10 13:35:30 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void signal_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	// cleanup(shell, 1);olmadı bir daha bak
	exit(130);
}

static void	heredoc_child(const char *delimiter, int pipefd[2], t_shell *shell)
{
	char	*line;

	signal(SIGINT, signal_heredoc);
	close(pipefd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter,
				(ft_strlen(delimiter) + 1)) == 0)
		{
			free(line);
			break;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]); // yazmayı kapat
	cleanup(shell, 0);
	shell = NULL;
	exit(0); // çocuktan çık
}

static int heredoc_parent(pid_t pid, int pipefd[2]) // PARENT: okuyucu
{
	int status;

	close(pipefd[1]); // yazma ucunu kapat
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, signal_catch);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipefd[0]);
		return (-1);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipefd[0]);
		return (-1);
	}
	return (pipefd[0]); // OKUMA ucu döndürülür, stdin'e bağlanmak için kullanılacak
}

static int handle_heredoc(const char *delimiter, t_shell *shell)
{
	int pipefd[2];
	pid_t pid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
		heredoc_child(delimiter, pipefd, shell);
	return (heredoc_parent(pid, pipefd));
}

int prepare_all_heredocs(t_ast_tree *node, t_shell *shell)
{
	t_redir *redir;

	if (!node)
		return (0);

	redir = node->redir_list;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			int fd = handle_heredoc(redir->target, shell);
			if (fd < 0)
				return (1);
			redir->fd = fd; // buraya fd'yi kaydet
		}
		redir = redir->next;
	}
	return (0);
}
