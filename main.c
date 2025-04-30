/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/04/30 18:58:11 by sakdil           ###   ########.fr       */
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

int	args_count(char **args)
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
	 int	arg_count;
	(void)argv;
	(void)argc;
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
		{
			//yazılanları historye ekle
			add_history(input);
		}
		char **args = ft_split(input, ' ');
		if (!args)
        {
            free(input);
            continue;
        }
		arg_count = args_count(args);
		if(builtin(arg_count, args, env) == 0)
		{	
			continue;
		}
		execute_command(input);
		free(input);
	}
	return (0);
}
