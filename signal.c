/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 15:19:50 by segunes           #+#    #+#             */
/*   Updated: 2025/08/13 21:00:42 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t	g_sigint_flag = 0;

void	signal_catch(int signum)
{
	(void)signum;
	g_sigint_flag = 1;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	ft_exit_code(130);
}

int	check_sigint_flag(void)
{
	if (g_sigint_flag)
	{
		g_sigint_flag = 0;
		return (1);
	}
	return (0);
}

void	signal_child(int signum)
{
	(void)signum;
	g_sigint_flag = 1;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
}
