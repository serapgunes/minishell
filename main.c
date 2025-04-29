/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/04/29 18:39:53 by segunes          ###   ########.fr       */
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


int	main(int argc, char **argv, char **env)
{
	 char	*input;

	while (1)
	{
		input = readline("minishell$ ");

		if (!input)
			break;
		if (*input)
			add_history(input);
		// if(builtin(argc, argv,env) == 0)
		//  	continue;


		if (strcmp(input,"exit"))
		if(is_only_spaces(input))
		{
			free(input);
			continue;
		}
		execute_command(input);
		free(input);
	}
	return (0);
}

