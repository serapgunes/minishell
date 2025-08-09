/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/09 22:11:12 by sakdil           ###   ########.fr       */
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
		// print_ast(ast, 0);// ast yazdırmak için
		//  print_tokens(tokens); type yazdırmak için
		if (prepare_all_heredocs(ast) != 0)
		{
			free_tokens(tokens); // Tokenları serbest bırak
			free(input);
			continue;
		}
		executor_structure(ast, &envp, 0);
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
