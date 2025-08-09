/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 20:54:37 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/09 21:18:31 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_pipe_split(t_token *tokens, t_token **left, t_token **right)
{
	t_token	*prev;

	*left = tokens;
	*right = NULL;
	prev = NULL;
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (!tokens->next)  // Pipe'tan sonra hiçbir şey yoksa
				return (1);
			*right = tokens->next;
			if (prev)
				prev->next = NULL;
			return (0);
		}
		prev = tokens;
		tokens = tokens->next;
	}
	return (0);
}

static int	count_words(t_token *cur)
{
	int	count;

	count = 0;
	while (cur)
	{
		if (cur->type == WORD)
			count++;
		cur = cur->next;
	}
	return (count);
}

static t_ast_tree	*build_cmd_node(t_token *left_token)
{
	t_ast_tree	*node;
	char		**args;
	int			count;

	count = count_words(left_token);
	node = create_cmd_node_with_args(count, &args);
	if (!node)
		return (NULL);
	if (fill_cmd_from_tokens(node, left_token, args) != 0)
	{
		free(args);
		free(node);
		return (NULL);
	}
	node->args = args;
	return (node);
}

static t_ast_tree	*make_pipe_node(t_token *left, t_token *right)
{
	t_ast_tree	*node;

	node = malloc(sizeof(t_ast_tree));
	if (!node)
		return (NULL);
	node->type = NODE_PIPE;
	node->redir_list = NULL;
	node->args = NULL;
	node->left = ft_build_ast(left);
	node->right = ft_build_ast(right);
	if (!node->left || !node->right)
	{
		printf("syntax error near unexpected token\n");
		free(node);
		return (NULL);
	}
	return (node);
}

t_ast_tree	*ft_build_ast(t_token *tokens)
{
	t_token	*left;
	t_token	*right;

	if (!tokens)
		return (NULL);
	if (find_pipe_split(tokens, &left, &right))
	{
		printf("syntax error near unexpected token\n");
		return (NULL);
	}
	if (right)
		return (make_pipe_node(left, right));
	return (build_cmd_node(tokens));
}
