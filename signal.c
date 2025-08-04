/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 15:19:50 by segunes           #+#    #+#             */
/*   Updated: 2025/08/04 20:07:59 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t sigint_flag = 0;

void	signal_catch(int signum)
{
	(void)signum;
	sigint_flag = 1;
	write(1, "\n", 1);      // ctrl c olduğunda yeni satıra geç
	rl_replace_line("", 0); // satırı temizle
	rl_on_new_line();       // yeni satır komutunu bildiriyor
	rl_redisplay();         // promptu yeniden çiziyor
}

int	check_sigint_flag(void)
{
	if (sigint_flag)
	{
		sigint_flag = 0;
		return (1);
	}
	return (0);
}
