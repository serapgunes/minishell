/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:20:45 by segunes           #+#    #+#             */
/*   Updated: 2025/08/04 00:01:50 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(int argc, char **argv)
{
	int i;
	size_t len;

	i = 1;
	while (i < argc)
	{
		len = strlen(argv[i]);
		if (write(STDOUT_FILENO, argv[i], len) < 0)
		{
			if (errno == EPIPE)
				return (1);
			else
			{
				perror("echo");
				return (1);
			}
		}
		if (i + 1 < argc)
		{
			if (write(STDOUT_FILENO, " ", 1) < 0)
			{
				if (errno == EPIPE)
					return (1);
				else
				{
					perror("echo");
					return (1);
				}
			}
		}
		i++;
	}
	if (write(STDOUT_FILENO, "\n", 1) < 0)
	{
		if (errno == EPIPE)
			return (1);
		else
		{
			perror("echo");
			return (1);
		}
	}
	return (0);
}
