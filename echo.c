/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:20:45 by segunes           #+#    #+#             */
/*   Updated: 2025/05/10 16:25:36 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ortam değişkenini bulup yazdırır
void print_env_var(char *var_name, char **env) {
	int i;

	i = 0;
	if (!env) 
		return; 
	while (env[i]) {
		char *equal = ft_strchr(env[i], '=');// = bunun adresini tutuyor
		if (equal && ft_strncmp(env[i], var_name, equal - env[i]) == 0 //strncmp ile var_name ve env[i] karşılaştırılıyor tamamen eşleşiyormu diye
			&& ft_strlen(var_name) == (size_t)(equal - env[i])) {
			printf("%s", equal + 1); // '=' sonrası değeri yazdırıyoruz
			return;
		}
		i++;
	}
	// Değişken yoksa bir şey yazma
}

void print_arg_cleaned(char *arg, char **env) {
	int j;
	int single;
	int in_double;
	int start;
	
	j = 0;
	single = 0;
	in_double = 0;
	while (arg[j]) {
		if (arg[j] == '\'' && !in_double) {
			single = !single;
			j++;
			continue;
		}
		if (arg[j] == '"' && !single) {
			in_double = !in_double;
			j++;
			continue;
		}
		if (arg[j] == '$' && !single) {
			j++; // '$' sonrası karaktere geç
			start = j;
			// Değişken adı okuma
			while (arg[j] && (ft_isalnum(arg[j]) || arg[j] == '_'))
				j++;

			if (j > start) {
				char var_name[256] = {0};
				ft_strlcpy(var_name, &arg[start], j - start);
				print_env_var(var_name, env);
				continue; // $ sonrası yazdırıldı, devam et
			}
			// $ yazılı ama geçerli değişken adı yoksa, direk '$' bas
			printf("$");
			continue;
		}
		printf("%c", arg[j]);
		j++;
	}
}

static void echo_skip(int argc, char **argv, char **env) {
	int i = 1;
	if (argc > 1 && ft_strcmp(argv[1], "-n") == 0)
		i = 2;

	while (i < argc) {
		print_arg_cleaned(argv[i],env);
		if (i < argc - 1)
			printf(" ");
		i++;
	}
}

int builtin_echo(int argc, char **argv, char **env) {
	int add_newline = 1;

	if (argc > 1 && ft_strcmp(argv[1], "-n") == 0)
		add_newline = 0;

	echo_skip(argc, argv, env);

	if (add_newline)
		printf("\n");

	return 0;
}
