/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 22:08:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/13 21:04:53 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	hd_write(int fd, char *line, int quoted, char **envp)
{
	char	*exp;

	if (!quoted)
	{
		exp = expand_variable(line, envp);
		if (exp)
		{
			write(fd, exp, ft_strlen(exp));
			write(fd, "\n", 1);
			free(exp);
			return ;
		}
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

static void	heredoc_child(const char *delim, int quoted,
				int pipefd[2], t_shell *shell)
{
	char	*line;

	signal(SIGINT, signal_heredoc);
	close(pipefd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line
			|| ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
		{
			free(line);
			break ;
		}
		hd_write(pipefd[1], line, quoted, shell->envp);
		free(line);
	}
	close(pipefd[1]);
	cleanup(shell, 0);
	exit(0);
}

static int	heredoc_parent(pid_t pid, int pipefd[2])
{
	int	status;

	close(pipefd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, signal_catch);
	if ((WIFEXITED(status) && WEXITSTATUS(status) == 130)
		|| (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT))
	{
		close(pipefd[0]);
		return (-1);
	}
	return (pipefd[0]);
}

static int	handle_heredoc(const char *delim, int quoted, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
		heredoc_child(delim, quoted, pipefd, shell);
	return (heredoc_parent(pid, pipefd));
}

static int	heredocs_prepare(t_ast_tree *node, t_shell *shell)
{	
	t_redir	*r;
	int		fd;

	r = node->redir_list;
	while (r)
	{
		if (r->type == HEREDOC)
		{
			fd = handle_heredoc(r->target, r->quoted, shell);
			if (fd < 0)
				return (1);
			r->fd = fd;
		}
		r = r->next;
	}
	return (0);
}

int	prepare_all_heredocs(t_ast_tree *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
	{
		if (prepare_all_heredocs(node->left, shell)) // Sol ve sağ alt ağaçlardaki heredocları da hazırlar(recursive)
			return (1);
		if (prepare_all_heredocs(node->right, shell))
			return (1);
		return (0);
	}
	return (heredocs_prepare(node, shell));
}
