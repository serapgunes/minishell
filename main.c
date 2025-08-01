/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/01 16:24:37 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_last_status = 0;

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
	// int		arg_count;
	t_list *history;
	t_token *tokens;
	t_ast_tree *ast;
	// int history_seen = 0;

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
			free(input);
			continue;
		}
		if (*input)
			add_history(input);
		char **args = ft_split(input, ' ');
		if (!args)
		{
			free(input);
			continue;
		}
		tokens = tokenize_input(input);
		if (!tokens)
		{
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
