/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:32:39 by segunes           #+#    #+#             */
/*   Updated: 2025/05/10 16:01:58 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin(int argc, char **argv, char **env, t_list *history)
{
	if (argc >= 1 && ft_strcmp(argv[0], "exit") == 0)
	{
		builtin_exit(argc, argv);
		return (0);
	}
	else if (ft_strcmp(argv[0], "env") == 0)
	{
		if (control_env(argc) == 1)
			builtin_env(env);
		return (0);
	}
	else if (ft_strcmp(argv[0], "history") == 0)
	{
		print_history(history);
		return (0);
	}
	else if (ft_strcmp(argv[0], "echo") == 0)
	{
		builtin_echo(argc, argv,env);
		return (0);
	}
	else if (ft_strcmp(argv[0], "cd") == 0)
	{
		builtin_cd(argc, argv);
		return (0);
	}
	else if (ft_strcmp(argv[0], "export") == 0)
	{
		builtin_export(argc, argv, env);
		return (0);
	}
	return (1);
}
