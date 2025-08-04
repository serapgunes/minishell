/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:20:45 by segunes           #+#    #+#             */
/*   Updated: 2025/08/04 20:00:11 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	coutinue_echo(int i, int argc, char **argv)
{
	size_t	len;

	len = strlen(argv[i]);
	if (write(STDOUT_FILENO, argv[i], len) < 0)
	{
		if (errno == EPIPE)
			return (1);
		perror("echo");
		return (1);
	}
	if (i + 1 < argc)
	{
		if (write(STDOUT_FILENO, " ", 1) < 0)
		{
			if (errno == EPIPE)
				return (1);
			perror("echo");
			return (1);
		}
	}
	return (0);
}

int	builtin_echo(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (coutinue_echo(i, argc, argv) != 0)
			return (1);
		i++;
	}
	if (write(STDOUT_FILENO, "\n", 1) < 0)
	{
		if (errno == EPIPE)
			return (1);
		perror("echo");
		return (1);
	}
	return (0);
}
