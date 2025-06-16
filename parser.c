/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:31:48 by segunes           #+#    #+#             */
/*   Updated: 2025/06/16 18:08:17 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_tree *ft_build_ast(t_token *tokens)
{
	t_token  *prev = NULL;
	t_token *right_token;
	t_token	*left_token;
	t_token  *current;
	
	current = tokens;
	left_token = tokens;	
	while(tokens)
	{
		if(tokens->type == PIPE)            
		{
			right_token = tokens->next;
			if(prev)
				prev->next =NULL;
			t_ast_tree *node = malloc(sizeof(t_ast_tree));
			node->type = NODE_PIPE;
			node->left = ft_build_ast(left_token);
			node->right = ft_build_ast(right_token);
			return node;
		}
		else
			tokens = tokens->next;		
	}
	t_ast_tree *node = malloc(sizeof(t_ast_tree));
	char **args = malloc(sizeof(char *));
	int i;

	i = 0;
	while(current)
	{
		if(current->type == WORD)
		{
			node->type = NODE_COMMAND;
			args[i++] = ft_strdup(current->value);		
		}
		else if(current->type == APPEND || current->type == HEREDOC || 
				current->type == REDIR_IN || current->type == REDIR_OUT)
		{
			node->redir_type = current->type;
			current = current->next;
			if (current && current->value)
				node->redir_target = ft_strdup(current->value);
				//bu yönlendirme operatöründen sonra gelen argümanı targetın içine koyuyoruz
		}
		else
		{
    		prev = tokens;
    		tokens = tokens->next;
		}

		current = current->next;
	}
	args[i] = NULL;
	node->args = args;
	node->left = NULL;
	node->right = NULL;
	return node;
	
}

int ft_last(t_token *input)
{
	t_token *last;

	last = input;

	while (last && last->next)
		last = last->next;
	if(last->type == APPEND || last->type == REDIR_IN || last->type == REDIR_OUT
		 || last->type == HEREDOC || last->type == PIPE)
		 {
		 	printf("syntax error near unexpected token");
			return (1);
		 }
	return (0);
}
int	is_invalid_redir_target(t_token *token)
{
	if (!token || token->type != WORD)
		return (1);
	if (!token->value || token->value[0] == '\0')
		return (1);
	if (token->type == REDIR_IN || token->type == REDIR_OUT
		|| token->type == APPEND || token->type == HEREDOC
		|| token->type == PIPE)
		return (1);
	return (0);
}
int ft_parser(t_token *input)
{	
	t_token *start;

	start = input;
	if (!input)
    	return (1);
	if(input->type == PIPE)
	{
		printf("syntax error near unexpected token");
		return (1);
	}
	while(input)
	{
		if(input->type == PIPE && input->next->type == PIPE)
		{
			printf("syntax error near unexpected token");
			return (1);
		}
		if((input->type == APPEND || input->type == REDIR_IN || input->type == REDIR_OUT
		 || input->type == HEREDOC) && is_invalid_redir_target(input->next))
		 {
		 	printf("syntax error near unexpected token");
			return (1);
		 }
		input = input->next;		
	}
	if (ft_last(start) == 1)
		return (1);
	return (0);
}

