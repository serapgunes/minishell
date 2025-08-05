/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 20:06:34 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/05 23:22:56 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_digit(int c)
{
	return (c >= '0' && c <= '9');
}

static int is_numeric(const char *str)
{
	int i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!is_digit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int is_too_large(char *str)
{
	int len;

	len = ft_strlen(str);
	if (len > 19)
		return (1);
	if (len == 19 && ft_strcmp(str, "9223372036854775807") > 0)
		return (1);
	return (0);
}
int builtin_exit(int argc, char **argv)
{
	int exit_code;

	exit_code = 0;
	ft_putendl_fd("exit", 1);
	if (argc > 2)
	{
		ft_putendl_fd("exit: too many arguments", 2);
		ft_exit_code(1);
		return (1);
	}
	if (argc == 2)
	{
		if (argc >= 2 && (!argv[1] || !is_numeric(argv[1]) || is_too_large(argv[1])))
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putendl_fd(": numeric argument required", 2);
			exit(2);
		}
		exit_code = ft_atoi(argv[1]) % 256;
	}
	ft_exit_code(exit_code);
	exit(exit_code);
	return (0);
}
