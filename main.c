/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/06 16:28:56 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int args_count(char **args)
{
	int i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int ft_exit_code(int temp)
{
	static int exit_code;

	if (temp == -1)
	{
		return (exit_code);
	}
	exit_code = temp;
	return (exit_code);
}

void signal_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(130);
}

int handle_heredoc(const char *delimiter)
{
	int pipefd[2];
	pid_t pid;
	char *line;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);

	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		signal(SIGINT, signal_heredoc);
		close(pipefd[0]);
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
		close(pipefd[1]); // yazmayı kapat
		exit(0);		  // çocuktan çık
	}
	else
	{
		// PARENT: okuyucu
		int status;

		close(pipefd[1]); // yazma ucunu kapat
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0); // çocuk bitsin
		signal(SIGINT, signal_catch);
		if (WEXITSTATUS(status) == 130)
			return -1;
		// OKUMA ucu döndürülür, stdin'e bağlanmak için kullanılacak
		return (pipefd[0]);
	}
}

int prepare_all_heredocs(t_ast_tree *node)
{
	t_redir *redir;

	if (!node)
		return (0);

	redir = node->redir_list;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			int fd = handle_heredoc(redir->target);
			if (fd < 0)
				return (1);
			redir->fd = fd; // buraya fd'yi kaydet
		}
		redir = redir->next;
	}
	return (0);
}

int main(int argc, char **argv, char **env)
{
	char *input;
	t_token *tokens;
	t_ast_tree *ast;
	char **envp = copy_env(env);

	(void)argv;
	(void)argc;
	ft_exit_code(0);
	while (1)
	{
		signal(SIGINT, signal_catch);
		input = readline("minishell$ ");

		if (input == NULL)
		{
			write(1, "exit\n", 5);
			cleanup_and_exit(input, tokens, ast);
		}
		if (!input)
			break;
		if (is_only_spaces(input))
		{
			add_history(input);
			free(input);
			continue;
		}
		if (*input)
			add_history(input);
		tokens = tokenize_input(input);
		if (!tokens)
		{
			ft_exit_code(0);
			free(input);
			continue;
		}
		if (ft_parser(tokens)) // Syntax kontrolü başarısızsa
		{
			free_tokens(tokens);
			free(input);
			continue;
		}
		ast = ft_build_ast(tokens);
		if (!ast)
		{
			free_tokens(tokens);
			free(input);
			continue;
		}
		// print_ast(ast, 0);// ast yazdırmak için
		//  print_tokens(tokens); type yazdırmak için
		if (prepare_all_heredocs(ast) != 0)
		{
			free_ast(ast);
			free_tokens(tokens); // Tokenları serbest bırak
			free(input);
			continue;
		}
		executor_structure(ast, &envp, 0);
		free_ast(ast);
		free_tokens(tokens);
		free(input);
	}
	return (ft_exit_code(-1));
}

// export yanlış çalışıyor
// minishell$ export VAR=42
// minishell$ env | grep VAR
// minishell$ export VAR2
// minishell$ export
