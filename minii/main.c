/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/04/19 22:29:11 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(char *input)
{
	pid_t	pid;
    char	*args[] = {input, NULL};
    
	pid = fork();
	if (pid < 0)
	{
		perror("fork hatasi");
		return;
	}
	else if (pid == 0)
	{
		execve(input, args, NULL);
		perror("execve hatasi");
		exit(1);
	}
	else
		wait(NULL);
}
}

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("minishell$ ");

		if (!input)
			break;

		if (*input)
			add_history(input);

		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			break;
		}

		execute_command(input);
		free(input);
	}
	return (0);
}

