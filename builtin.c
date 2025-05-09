/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:32:39 by segunes           #+#    #+#             */
/*   Updated: 2025/05/09 18:22:48 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	builtin_exit(int argc, char **argv)
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

static int control_env(int argc)
{
	if(argc > 1)
	{	
		write(1, "command not found\n", 18);
		return (0);
	}
	return(1);	
}

static int builtin_env(char **env)
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

void echo_skip(int argc, char **argv)
{
	int i;
	int j;

	i =1;
	if (ft_strcmp(argv[1], "-n") == 0)
	 	i = 2;
	while (i < argc)
	{
		j=0;
		while(argv[i][j])
		{
			if(ft_strcmp(argv[i], 34) != 0 && ft_strcmp(argv[i], 39) == 0)
			{
				if(argv[i][j] != 39)
					j++;
			}
			while(argv[i][j] == '"')
				j++;
			printf("%c", argv[i][j]);
			if (i < (argc - 1))
				printf(" "); 		//argümanlar arası bir boşluk olması için
			j++;
		}
		i++;
	}
}

static int builtin_echo(int argc, char **argv)
{
	int	i;
	int	add_newline;

	i = 1;
	add_newline = 1;
	if (argc > 1 && ft_strcmp(argv[1], "-n") == 0)
	{
		add_newline = 0;
		i++;
	}
	echo_skip(argc, argv);
	if (add_newline == 1)
		printf("\n");
	return (0);
}

//chdir (change directory): geçerli çalışma dizinini belirtilen bir yola değiştiren sistem çağrısı
// unistd.h'ta var

static int	builtin_cd(int argc, char **argv)
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
		builtin_echo(argc, argv);
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
