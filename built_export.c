/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:27:34 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/06 15:03:05 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(const char *name)
{
	int	i;

	i = 0;
	if (!name || !name[0])
		return (0);
	if (name[0] >= '0' && name[0] <= '9')
		return (0);
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int	is_valid_identifier_len(const char *name, size_t len)
{
	size_t	i;

	i = 0;
	if (len == 0 || (name[0] >= '0' && name[0] <= '9'))
		return (0);
	while (i < len)
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int	process_export_assignment(char *arg, char ***envp)
{
	char	*name;
	int		len;
	int		status;

	status = 0;
	len = ft_strchr(arg, '=') - arg;
	if (!is_valid_identifier_len(arg, len))
	{
		name = ft_substr(arg, 0, len);
		if (name)
		{
			print_invalid_identifier(name);
			free(name);
		}
		status = 1;
	}
	else if (set_env_var(arg, envp))
		status = 1;
	return (status);
}

static int	process_export_name(char *arg, char ***envp)
{
	int		status;
	char	*entry;

	status = 0;
	if (!arg || arg[0] == '\0')
	{
		print_invalid_identifier(arg);
		return (1);
	}
	if (arg[0] == '=' || ft_strchr(arg, '=') == arg)
	{
		print_invalid_identifier(arg);
		return (1);
	}
	if (!is_valid_identifier(arg))
	{
		print_invalid_identifier(arg);
		status = 1;
	}
	else if (find_in_environ(arg, *envp) < 0)
	{
		entry = ft_strdup(arg);
		if (!entry || extend_env(envp, entry))
			status = 1;
	}
	return (status);
}

int	builtin_export(int argc, char **argv, char ***envp)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	if (argc == 1)
	{
		print_sorted_env(*envp);
		return (0);
	}
	while (i < argc)
	{
		if (argv[i] && ft_strchr(argv[i], '='))
		{
			if (process_export_assignment(argv[i], envp))
				status = 1;
		}
		else
		{
			if (process_export_name(argv[i], envp))
				status = 1;
		}
		i++;
	}
	return (status);
}
