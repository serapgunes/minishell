/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:55:04 by segunes           #+#    #+#             */
/*   Updated: 2025/06/17 15:27:40 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void executor_structure(t_ast_tree *node, char **envp)
{
	if(node->type == NODE_COMMAND)
	{

	}
	else if(node->type == PIPE)
	{

	}
	else if(node->type == REDIR_IN || node->type == REDIR_OUT)
	{

	}
	else if(node->type == APPEND || node->type == HEREDOC)
	{
		
	}
}