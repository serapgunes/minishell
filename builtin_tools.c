/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:28:23 by segunes           #+#    #+#             */
/*   Updated: 2025/05/12 16:12:12 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ;

int	builtin_exit(int argc, char **argv)
{
	(void)argv;
	if (argc > 1)
	{
		write(2, "exit: no arguments allowed\n", 27);
		return (1);
	}
	printf("exit\n");
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
    int i = 0;

    (void)env;
    while (environ[i])
        printf("%s\n", environ[i++]);
    return (0);
}

//chdir (change directory): geçerli çalışma dizinini belirtilen bir yola değiştiren sistem çağrısı
// unistd.h'ta var

int	builtin_cd(int argc, char **argv)
{
	if (argc > 2)
	{
		printf("cd : Too many arguments\n");
		return (1);
	}
	if (argc == 1) //eğer sadece cd varsa bir şey yapmayıp geri dönecek(normal terminalde böyle değil ama pdf'te yapın dememiş)
		return (0);
	if (chdir(argv[1]) != 0) 	//argv[1]'deki yola dizini değiştirmeye ççalışır. (0 başarılı, 1 başarısız)
	{
		printf("cd : %s No such file or directory\n", argv[1]);
		return (1);
	}
	return (0);
}
