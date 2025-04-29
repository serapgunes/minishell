/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:32:39 by segunes           #+#    #+#             */
/*   Updated: 2025/04/29 18:08:26 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_exit(int argc, char **argv)
{
	if(argc >= 2)
	{	
		write(1, "exit: too many arguments\n", 25);
		return (0);
	}
	else if(ft_isdigit(argv[0]) == 1)
	{
		write(1, "exit: numeric argument required\n", 32);
		free(argv);
		exit(255);
	}
	return(0);	
}

int control_env(int argc,char **argv)
{
	if(argc >= 2 || ft_strcmp(argv[0], "env") != 0)
	{	
		write(1, "command not found", 17);
		return (0);
	}
	return(0);	
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
	if(ft_strcmp(argv[1], "exit") == 0)
		return (builtin_exit(argc,argv));
	else if(ft_strcmp(argv[0], "env") == 0)
	{
		control_env(argc,argv);
		builtin_env(env);
	}
	return 1;
}
