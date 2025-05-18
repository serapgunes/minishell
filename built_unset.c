/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:03:45 by sakdil            #+#    #+#             */
/*   Updated: 2025/05/18 21:25:57 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ;

static int	alloc_new_env(char ***new_env, int size)
{
	*new_env = malloc(sizeof(char *) * size);
	if (!*new_env)
	{
		printf("unset: memory allocation failed\n");
		return (1);
	}
	return (0);
}

static int	remove_from_environ(int idx)
{
	int		i;
	int		count;
	char	**new_env;

	count = 0;
	while (environ[count])
		count++;
	if (alloc_new_env(&new_env, count) != 0)
		return (1);
	i = 0;
	count = 0;
	while (environ[i])
	{
		if (i != idx)
			new_env[count++] = environ[i];
		else
			free(environ[i]);
		i++;
	}
	new_env[count] = NULL;
	free(environ);
	environ = new_env;
	return (0);
}


static int	unset_one_var(char *var, int *status)
{
	int idx;

	if (!is_valid_identifier(var))
	{
		printf("unset: '%s': not a valid identifier\n", var);
		*status = 1;
		return (1);
	}
	idx = find_in_environ(var);
	if (idx >= 0)
	{
		if (remove_from_environ(idx) != 0)
			*status = 1;
	}
	return (0);
}

int	builtin_unset(int argc, char **argv)
{
	int	i;
	int	status;

    status = 0;
	i = 1;
	if (argc < 2)
		return (0);
	while (i < argc)
	{
		unset_one_var(argv[i], &status);
		i++;
	}
	return (status);
}
