/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:20:45 by segunes           #+#    #+#             */
/*   Updated: 2025/05/10 14:31:37 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_arg_cleaned(char *arg) {
	int j = 0;
	int in_single_quote = 0;
	int in_double_quote = 0;

	while (arg[j]) {
		if (arg[j] == '\'' && !in_double_quote) {
			in_single_quote = !in_single_quote; // Toggle single quote mode
			j++;
			continue;
		}
		if (arg[j] == '"' && !in_single_quote) {
			in_double_quote = !in_double_quote; // Toggle double quote mode
			j++;
			continue;
		}
		printf("%c", arg[j]);
		j++;
	}
}

static void echo_skip(int argc, char **argv) {
	int i = 1;
	if (argc > 1 && ft_strcmp(argv[1], "-n") == 0)
		i = 2;

	while (i < argc) {
		print_arg_cleaned(argv[i]);
		if (i < argc - 1)
			printf(" ");
		i++;
	}
}

int builtin_echo(int argc, char **argv) {
	int add_newline = 1;

	if (argc > 1 && ft_strcmp(argv[1], "-n") == 0)
		add_newline = 0;

	echo_skip(argc, argv);

	if (add_newline)
		printf("\n");

	return 0;
}
