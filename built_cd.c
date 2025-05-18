/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 20:09:08 by sakdil            #+#    #+#             */
/*   Updated: 2025/05/18 20:09:23 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_cd(int argc, char **argv)
{
	char	*home;

	if (argc > 2)
	{
		printf("cd: too many arguments\n");
		return (1);
	}
	if (argc == 1)
	{
		home = getenv("HOME");
		if (!home)
		{
			printf("cd: HOME not set\n");
			return (1);
		}
		if (chdir(home) != 0)
		{
			printf("cd: %s: No such file or directory\n", home);
			return (1);
		}
		return (0);
	}
	if (chdir(argv[1]) != 0)
	{
		printf("cd: %s: No such file or directory\n", argv[1]);
		return (1);
	}
	return (0);
}
