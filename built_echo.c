/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:20:45 by segunes           #+#    #+#             */
/*   Updated: 2025/08/05 15:52:41 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	n_flag(int argc, char **argv, int *i)
{
	int	nflag;
	int	j;

	nflag = 0;
	while (*i < argc && argv[*i][0] == '-' && argv[*i][1] == 'n')
	{
		j = 1;
		while (argv[*i][j] == 'n')
			j++;
		if (argv[*i][j] != '\0')
			break;
		nflag = 1;
		(*i)++;
	}
	return (nflag);
}

static int	continue_echo(int i, int argc, char **argv)
{
	size_t	len;

	len = strlen(argv[i]);
	if (write(1, argv[i], len) < 0)
	{
		if (errno == EPIPE)
			return (1);
		perror("echo");
		return (1);
	}
	if (i + 1 < argc)
	{
		if (write(1, " ", 1) < 0)
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
	int	nflag;

	i = 1;
	nflag = n_flag(argc, argv, &i);
	while (i < argc)
	{
		if (continue_echo(i, argc, argv) != 0)
			return (1);
		i++;
	}
	if (!nflag)
	{
		if (write(1, "\n", 1) < 0)
		{
			if (errno == EPIPE)
				return (1);
			perror("echo");
			return (1);
		}
	}
	return (0);
}

/*
mini: 
minishell$ echo $A AAAA
 AAAA

minishell$ echo $A1 AAAA
1 AAAA

minishell$ echo $A/AAA AAAA
 AAAA

minishell$ echo $1/AAA AAAA (DOĞRU)
/AAA AAAA 

minishell$ echo $/AAA AAAA
AAA AAAA

*/