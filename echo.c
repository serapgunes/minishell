/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:20:45 by segunes           #+#    #+#             */
/*   Updated: 2025/05/10 22:21:08 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_env_var(char *var_name, char **env) {
	int i;

	i = 0;
	if (!env) 
		return; 
	while (env[i]) {
		char *equal = ft_strchr(env[i], '=');
		if (equal && ft_strncmp(env[i], var_name, equal - env[i]) == 0
			&& ft_strlen(var_name) == (size_t)(equal - env[i]))
			{
			printf("%s", equal + 1); // '=' sonrası değer
			return;
		}
		i++;
	}
	// Değişken yoksa bir şey yazma (Bash gibi)
}

// echo içindeki argümanları işler
void print_arg_cleaned(char *arg, char **env) {
	int j = 0;
	int in_single_quote = 0;
	int in_double_quote = 0;

	while (arg[j]) {
		if (arg[j] == '\'' && !in_double_quote) {
			in_single_quote = !in_single_quote;
			j++;
			continue;
		}
		if (arg[j] == '"' && !in_single_quote) {
			in_double_quote = !in_double_quote;
			j++;
			continue;
		}
		if (arg[j] == '$' && !in_single_quote) {
			j++; // '$' sonrası karaktere geç
			int start = j;
			// Değişken adı okuma
			while (arg[j] && (isalnum(arg[j]) || arg[j] == '_'))
				j++;

			if (j > start) {
				char var_name[256] = {0};
				ft_strncpy(var_name, &arg[start], j - start);
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