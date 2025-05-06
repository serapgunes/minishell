/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/05/06 13:52:38 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(char *input)
{
	pid_t	pid;

	char **args;
	
	args =  ft_split(input , ' ');
	pid = fork();
	if (pid < 0)
	{
		perror("fork hatasi");
		return;
	}
	else if (pid == 0)
	{
		execve(pathname(args[0]), args, NULL);
		perror("execve hatasi");
		exit(1);
	}
	else
		wait(NULL);
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
		if(is_only_spaces(input))
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
		arg_count = args_count(args);
		if (builtin(arg_count, args, env, history) == 0)
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
