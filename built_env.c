/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 20:10:19 by sakdil            #+#    #+#             */
/*   Updated: 2025/05/18 21:18:19 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ;

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
    (void)env;

    while (environ[i])
    {
        if (ft_strchr(environ[i], '='))
            printf("%s\n", environ[i]);
        i++;
    }
    return (0);
}
