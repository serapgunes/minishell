/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 21:18:30 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/10 21:43:02 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_with_cmd_error(char *cmd, char *msg, int code, t_shell *shell)
{
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(msg, 2);
	cleanup(shell, 0);
	shell = NULL;
	exit(code);
}

void	restore_std(t_std std)
{
	dup2(std.std_in, STDIN_FILENO);
	dup2(std.std_out, STDOUT_FILENO);
	close(std.std_in);
	close(std.std_out);
}
