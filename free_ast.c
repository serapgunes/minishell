/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:13:13 by segunes           #+#    #+#             */
/*   Updated: 2025/08/06 14:26:28 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_redir_list(t_redir *redir)
{
	t_redir *tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->target); // target malloc ile ayrıldıysa
		free(redir);
		redir = tmp;
	}
}

void free_ast(t_ast_tree *node)
{
	int i;

	if (!node)
		return;

	free_ast(node->left);
	free_ast(node->right);

	if (node->args)
	{
		i = 0;
		while (node->args[i] != NULL)
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}

	if (node->redir_list)
		free_redir_list(node->redir_list);

	free(node);
}
