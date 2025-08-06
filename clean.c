/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:28:20 by segunes           #+#    #+#             */
/*   Updated: 2025/08/06 17:41:31 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cleanup_and_exit(char *input, t_token *tokens, t_ast_tree *ast, int exit_code)
{
	free_ast(ast);
	free_tokens(tokens);
	free(input);
	rl_clear_history();
	exit(exit_code);
}