/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 20:10:19 by sakdil            #+#    #+#             */
/*   Updated: 2025/06/26 16:56:29 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// extern char **environ;

int control_env(int argc)
{
	if (argc > 1)
	{	
		write(1, "command not found\n", 18);
		return (0);
	}
	return(1);	
}

int builtin_env(char **env)
{
    int i;

	i = 0;
    // (void)env;

    while (env[i])
    {
        if (ft_strchr(env[i], '='))
            printf("%s\n", env[i]);
        i++;
    }
    return (0);
}
