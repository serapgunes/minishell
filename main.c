/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/06/01 19:11:53 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_status = 0;

void execute_command(char *input)
{
    pid_t pid;
    char **args;
    const char *path;

    args = ft_split(input, ' ');
    if (!args || !args[0])
    {
        //free_args(args);
        return;
    }
    path = pathname(args[0]);
    if (!path)
    {
        printf("%s: command not found\n", args[0]);
       // free_args(args);
        return;
    }
    pid = fork();
    if (pid < 0)
    {
        perror("fork hatasi");
        //free_args(args);
        return;
    }
    else if (pid == 0)
    {
        execve(path, args, NULL);
        perror("execve hatasi");
        exit(1);
    }
    else
        wait(NULL);
    //free_args(args);
}

static int	args_count(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	int		arg_count;
	t_list *history;
	int history_seen = 0;
	
	(void)argv;
	(void)argc;
	history = NULL;
	while (1)
	{
		input = readline("minishell$ ");
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
		// t_token *tokens = tokenize_input(input);
		// print_tokens(tokens); type yazdırmak için
		arg_count = args_count(args);
		int builtin_result = builtin(arg_count, args, env, history);
		if (builtin_result == 0 || builtin_result == 1 || builtin_result == 2)
		{
			if (ft_strcmp(args[0], "history") != 0)
			{
				if (history_seen == 1)
					add_to_history(&history, input);
				history_seen = 1;
			}
			else
				add_to_history(&history, input);
			//free_args
			continue;
		}
		add_to_history(&history, input);
		execute_command(input);
		free(input);
	}
	//free_history
	return (0);
}
//ls çalışmıyor çünkü execute builtinden sonra oyüzden görmüyor

// denemek için
// int	main(int argc, char **argv, char **envp)
// {
// 	char	*input;
// 	t_token	*tokens;
// 	(void)envp;

// 	(void)argc;
// 	(void)argv;
// 	while (1)
// 	{
// 		input = readline("minishell$ ");
// 		if (!input)
// 			break ;
// 		if (*input)
// 			add_history(input);
// 		tokens = tokenize_input(input);
// 		if (tokens)
// 		{
// 			print_tokens(tokens);
// 			free_tokens(tokens);
// 		}
// 		free(input);
// 	}
// 	return (0);
// }
