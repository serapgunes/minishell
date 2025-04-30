/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:32:39 by segunes           #+#    #+#             */
/*   Updated: 2025/04/30 18:59:22 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_exit(int argc, char **argv)
{
	if(argc > 2)
	{	
		write(1, "exit: too many arguments\n", 25);
		return (0);
	}
	if (argc == 2)
    {
        if (!ft_isdigit(argv[1]))
        {
            write(1, "exit: numeric argument required\n", 32);
            return (0);
        }
        exit(1);
    }
    exit(0);
}

int control_env(int argc)
{
	if(argc > 1)
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
    while (env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
    return (1);
}

int builtin(int argc, char **argv, char **env)
{
	if(argc >= 1 && ft_strcmp(argv[0], "exit") == 0)
	{
		return (builtin_exit(argc,argv));
	}	
	else if (ft_strcmp(argv[0], "env") == 0)
	{
		if (control_env(argc) == 1)
			return (builtin_env(env));
		return (0);
	}
	else if (ft_strcmp(argv[0], "history") == 0)
	{
		//print history
	}

	return (1);
}
