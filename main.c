/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/05 15:48:27 by sakdil           ###   ########.fr       */
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
		close(pipefd[0]);
		while (1)
		{
			line = readline("heredoc> ");
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

		close(pipefd[1]);		  // yazma ucunu kapat
		waitpid(pid, &status, 0); // çocuk bitsin

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
	// int		arg_count;
	t_list *history;
	t_token *tokens;
	t_ast_tree *ast;
	//  int history_seen = 0;

	(void)argv;
	(void)argc;
	history = NULL;
	ft_exit_code(0);
	signal(SIGINT, signal_catch);
	while (1)
	{
		input = readline("minishell$ ");
		if (check_sigint_flag())
		{
			ft_exit_code(130);
			continue;
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
			free(input);
			continue;
		}
		if (!tokens) // Ekstra kontrol: yine NULL olabilir!
		{
			free(input);
			continue;
		}
		ast = ft_build_ast(tokens);
		if (!ast)
		{
			free(input);
			continue;
		}
		// print_ast(ast, 0);// ast yazdırmak için
		//  print_tokens(tokens); type yazdırmak için

		// buraya koy heredocccc
		if (prepare_all_heredocs(ast) != 0)
		{
			printf("sıkıntı heredoc döngüsünde\n");
			continue;
		}
		executor_structure(ast, env, 0);
		// printf("minishell exit_status = %d\n", exit_status);
		//   arg_count = args_count(args);
		//   int builtin_result = builtin(arg_count, args, env, history);
		//   if (builtin_result == 0 || builtin_result == 1 || builtin_result == 2)
		//   {
		//   	if (ft_strcmp(args[0], "history") != 0)
		//   	{
		//   		if (history_seen == 1)
		//   			add_to_history(&history, input);
		//   		history_seen = 1;
		//   	}
		//   	else
		//   		add_to_history(&history, input);
		//   	//free_args
		//   	continue;
		//   }
		add_to_history(&history, input);
		// execute_command(input);
		free(input);
	}
	return (ft_exit_code(-1));
	// return (exit_status);
	//    free_history
}

// minishell$ echo -n hello
// -n hello
// minishell$ echo -n -n hello
// -n -n hello
// minishell$
// bu kısım yanlış çalışıyor

// export yanlış çalışıyor
// minishell$ export VAR=42
// minishell$ env | grep VAR
// minishell$ export VAR2
// minishell$ export

// bu yönlendirme yanlış
// minishell$ cat << EOF
// > serap
// > EOF
// > cat
// > file
// > EOF
// cat
// file