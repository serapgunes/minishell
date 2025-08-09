/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/09 22:41:39 by segunes          ###   ########.fr       */
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

void ft_free(char **envp)
{
	int i = 0;

	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

void free_redir_list(t_redir *redir)
{
	t_redir *tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;

		// Eğer target varsa, onu serbest bırak
		if (tmp->target)
			free(tmp->target);

		free(tmp); // Redir node'unu serbest bırak
	}
}

void free_ast_tree(t_ast_tree *node)
{
	if (!node)
		return;

	// args dizisini serbest bırak
	if (node->args)
	{
		int i = 0;
		while (node->args[i]) // args dizisindeki her stringi free et
		{
			free(node->args[i]);
			i++;
		}
		free(node->args); // args dizisini free et
	}

	// Redirection listesindeki her elemanı serbest bırak
	if (node->redir_list)
		free_redir_list(node->redir_list);

	// Sol ve sağ alt ağaçları serbest bırak (recursive)
	if (node->left)
		free_ast_tree(node->left);
	if (node->right)
		free_ast_tree(node->right);

	// Node'un kendisini serbest bırak
	free(node);
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
			exit(0);
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
		// exit(123);
		//  print_ast(ast, 0);// ast yazdırmak için
		//   print_tokens(tokens); type yazdırmak için
		if (prepare_all_heredocs(ast) != 0)
		{
			free_tokens(tokens); // Tokenları serbest bırak
			free_ast_tree(ast);
			free(input);
			continue;
		}
		executor_structure(ast, &envp, 0);
		// ft_free(envp);
		if (ast)
			free_ast_tree(ast);
		if (tokens)
			free_tokens(tokens);
		free(input);
	}
	if (envp)
		ft_free(envp);

	return (ft_exit_code(-1));
}

// export yanlış çalışıyor
// minishell$ export VAR=42
// minishell$ env | grep VAR
// minishell$ export VAR2
// minishell$ export
// valgrind --leak-check=full --show-leak-kinds=all  --suppressions=./readline.supp ./minishell