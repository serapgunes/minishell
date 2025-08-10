/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:03:45 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/10 23:19:45 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_env_index(char ***envp, int idx)
{
	char	**env;
	int		i;

	env = *envp;
	free(env[idx]);
	i = idx;
	while (env[i])
	{
		env[i] = env[i + 1];
		i++;
	}
	*envp = env;
}

int	builtin_unset(int argc, char **argv, char ***envp)
{
	int	i;
	int	idx;
	int	status;

	status = 0;
	i = 1;
	while (i < argc)
	{
		if (!is_valid_identifier(argv[i]))
		{
			print_invalid_identifier(argv[i]);
			status = 1;
		}
		else
		{
			while ((idx = find_in_environ(argv[i], *envp)) >= 0)
				remove_env_index(envp, idx);
		}
		i++;
	}
	return (status);
}

