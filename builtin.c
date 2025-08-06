/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:32:39 by segunes           #+#    #+#             */
/*   Updated: 2025/08/06 18:00:07 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "unset"));
}

int builtin(int argc, char **argv, char ***env, char *input, t_token *tokens, t_ast_tree *ast)
{
	if (argc >= 1 && argv[0] && ft_strcmp(argv[0], "exit") == 0)
		return (builtin_exit(argc, argv, input, tokens, ast));
	else if (ft_strcmp(argv[0], "env") == 0)
	{
		if (control_env(argc) == 1)
			builtin_env(env);
		return (0);
	}
	else if (ft_strcmp(argv[0], "echo") == 0)
		return (builtin_echo(argc, argv));
	else if (ft_strcmp(argv[0], "cd") == 0)
		return (builtin_cd(argc, argv));
	else if (ft_strcmp(argv[0], "export") == 0)
		return (builtin_export(argc, argv, env));
	else if (ft_strcmp(argv[0], "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(argv[0], "unset") == 0)
		return (builtin_unset(argc, argv, env));
	return (-1);
}
