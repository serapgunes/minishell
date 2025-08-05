/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:31:48 by segunes           #+#    #+#             */
/*   Updated: 2025/08/04 20:14:47 by sakdil           ###   ########.fr       */
/*       																	  */
/* ************************************************************************** */

#include "minishell.h"

char *strip_quotes(char *str)
{
	size_t len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len - 1] == '\''))
		return (ft_substr(str, 1, len - 2));
	return (ft_strdup(str));
}

t_ast_tree *ft_build_ast(t_token *tokens)
{
	t_token *prev = NULL;
	t_token *right_token;
	t_token *left_token;
	t_token *current;
	int count;

	count = 0;
	current = tokens;
	left_token = tokens;
	if (!tokens)
		return NULL;
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (!tokens->next) // Pipe'tan sonra hiçbir şey yoksa
			{
				printf("syntax error near unexpected token\n");
				return NULL;
			}
			right_token = tokens->next;
			if (prev)
				prev->next = NULL;
			t_ast_tree *node = malloc(sizeof(t_ast_tree));
			if (!node)
				return NULL;
			node->type = NODE_PIPE;
			node->redir_list = NULL;
			node->args = NULL;
			node->left = ft_build_ast(left_token);
			node->right = ft_build_ast(right_token);
			if (!node->left || !node->right)
			{
				printf("syntax error near unexpected token\n");
				free(node);
				return NULL;
			}
			return node;
		}
		// else
		// 	tokens = tokens->next;
		prev = tokens;
		tokens = tokens->next;
	}
	t_ast_tree *node = malloc(sizeof(t_ast_tree));
	if (!node)
		return NULL;

	// Node'u initialize et
	node->type = NODE_COMMAND;
	node->redir_list = NULL;
	node->left = NULL;
	node->right = NULL;

	// Args için bellek ayır
	current = left_token;
	while (current != NULL)
	{
		if (current->type == WORD)
			count++;
		current = current->next;
	}
	char **args = malloc(sizeof(char *) * (count + 1));
	// value yi saydırıp ona göre yer aç
	if (!args)
	{
		free(node);
		return NULL;
	}
	int i;

	current = left_token;
	i = 0;
	while (current)
	{
		if (current->type == WORD)
		{
			node->type = NODE_COMMAND;
			args[i++] = ft_strdup(current->value);
		}
		else if (current->type == APPEND || current->type == HEREDOC ||
				 current->type == REDIR_IN || current->type == REDIR_OUT)
		{
			t_redir *new_redir = malloc(sizeof(t_redir));
			if (!new_redir)
				return NULL; // veya exit(1)
			new_redir->type = current->type;
			current = current->next;
			if (current && current->value)
			{
				char *filename = strip_quotes(current->value);
				filename = normalize_filename(filename);
				new_redir->target = filename;
			}
			else
				new_redir->target = NULL;

			new_redir->next = NULL;

			// Listeye ekle (redir_list'e)
			if (!node->redir_list)
				node->redir_list = new_redir;
			else
			{
				t_redir *tmp = node->redir_list;
				while (tmp && tmp->next)
					tmp = tmp->next;
				if (tmp)
					tmp->next = new_redir;
			}
			if (current) // buraya ekledik
				current = current->next;
			continue;
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
	if (last->type == APPEND || last->type == REDIR_IN || last->type == REDIR_OUT || last->type == HEREDOC || last->type == PIPE)
	{
		printf("syntax error near unexpected token\n");
		return (1);
	}
	return (0);
}
int is_invalid_redir_target(t_token *token)
{
	if (!token || token->type != WORD)
		return (1);
	if (!token->value || token->value[0] == '\0')
		return (1);
	if (token->type == REDIR_IN || token->type == REDIR_OUT || token->type == APPEND || token->type == HEREDOC || token->type == PIPE)
		return (1);
	return (0);
}

int ft_parser(t_token *input)
{
	t_token *start;

	start = input;
	if (!input)
		return (1);
	if (input->type == PIPE)
	{
		printf("syntax error near unexpected token\n");
		return (1);
	}
	while (input)
	{
		if (input->type == PIPE && input->next == NULL)
		{
			printf("syntax error near unexpected token\n");
			return (1);
		}
		if (input->type == PIPE && input->next->type == PIPE)
		{
			printf("syntax error near unexpected token\n");
			return (1);
		}
		if ((input->type == APPEND || input->type == REDIR_IN || input->type == REDIR_OUT || input->type == HEREDOC) && is_invalid_redir_target(input->next))
		{
			printf("syntax error near unexpected token\n");
			return (1);
		}
		input = input->next;
	}
	if (ft_last(start) == 1)
		return (1);
	return (0);
}

// önceki kod
// t_ast_tree *ft_build_ast(t_token *tokens)
// {
// 	t_token *prev = NULL;
// 	t_token *right_token;
// 	t_token *left_token;
// 	t_token *current;

// 	current = tokens;
// 	left_token = tokens;
// 	while (tokens)
// 	{
// 		if (tokens->type == PIPE)
// 		{
// 			if (!tokens->next) // Pipe'tan sonra hiçbir şey yoksa
// 			{
// 				printf("syntax error near unexpected token\n");
// 				return NULL;
// 			}
// 			right_token = tokens->next;
// 			if (prev)
// 				prev->next = NULL;
// 			t_ast_tree *node = malloc(sizeof(t_ast_tree));
// 			node->type = NODE_PIPE;
// 			node->left = ft_build_ast(left_token);
// 			node->right = ft_build_ast(right_token);
// 			if (!node->left || !node->right)
// 			{
// 				printf("syntax error near unexpected token\n");
// 				// free(node);
// 				return NULL;
// 			}
// 			return node;
// 		}
// 		// else
// 		// 	tokens = tokens->next;
// 		prev = tokens;
// 		tokens = tokens->next;
// 	}
// 	t_ast_tree *node = malloc(sizeof(t_ast_tree));
// 	char **args = malloc(sizeof(char *));
// 	int i;

// 	i = 0;
// 	while (current)
// 	{
// 		if (current->type == WORD)
// 		{
// 			node->type = NODE_COMMAND;
// 			args[i++] = ft_strdup(current->value);
// 		}
// 		else if (current->type == APPEND || current->type == HEREDOC ||
// 				 current->type == REDIR_IN || current->type == REDIR_OUT)
// 		{
// 			t_redir *new_redir = malloc(sizeof(t_redir));
// 			if (!new_redir)
// 				return NULL; // veya exit(1)

// 			new_redir->type = current->type;
// 			current = current->next;
// 			// if (current && current->value)
// 			// 	//new_redir->target = ft_strdup(current->value);
// 			// 	new_redir->target = strip_quotes(current->value);

// 			// else
// 			// 	new_redir->target = NULL;
// 			if (current && current->value)
// 			{
// 				char *filename = strip_quotes(current->value);
// 				filename = normalize_filename(filename);
// 				new_redir->target = filename;
// 			}
// 			else
// 				new_redir->target = NULL;

// 			new_redir->next = NULL;

// 			// Listeye ekle (redir_list'e)
// 			if (!node->redir_list)
// 				node->redir_list = new_redir;
// 			else
// 			{
// 				t_redir *tmp = node->redir_list;
// 				while (tmp->next)
// 					tmp = tmp->next;
// 				tmp->next = new_redir;
// 			}
// 		}

// 		else
// 		{
// 			prev = tokens;
// 			tokens = tokens->next;
// 		}

// 		current = current->next;
// 	}
// 	args[i] = NULL;
// 	node->args = args;
// 	node->left = NULL;
// 	node->right = NULL;
// 	return node;
// }

//***************************************************************************************/
// ast yapısı doğrumu diye kontrol ediliyor burada yazdırmak için//
// void print_ast(t_ast_tree *node, int depth)
// {
// 	if (!node)
// 		return;

// 	for (int i = 0; i < depth; i++)
// 		printf("  "); // girinti

// 	if (node->type == NODE_PIPE)
// 		printf("PIPE\n");
// 	else if (node->type == NODE_COMMAND)
// 	{
// 		printf("CMD: ");
// 		for (int i = 0; node->args && node->args[i]; i++)
// 			printf("%s ", node->args[i]);
// 		printf("\n");

// 		if (node->redir_type)
// 		{
// 			for (int i = 0; i < depth + 1; i++)
// 				printf("  ");
// 			printf("REDIR: %d -> %s\n", node->redir_type, node->redir_target);
// 		}
// 	}

// 	if (node->left)
// 		print_ast(node->left, depth + 1);
// 	if (node->right)
// 		print_ast(node->right, depth + 1);
// }
//***************************************************************************************/