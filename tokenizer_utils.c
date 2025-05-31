/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 17:51:21 by segunes           #+#    #+#             */
/*   Updated: 2025/05/31 17:54:24 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *create_word_token(char *value)
{
	t_token *new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = WORD;
	new_token->value = value;
	new_token->next = NULL;
	return (new_token);
}

t_token *create_pipe_token(char *value)
{
	t_token *new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = PIPE;
	new_token->value = value;
	new_token->next = NULL;
	return (new_token);
}

t_token *create_redir_token(char *value)
{
	t_token *new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	if(value[0] == '<')
		new_token->type = REDIR_IN;
	else
		new_token->type = REDIR_OUT;
	new_token->value = value;
	new_token->next = NULL;
	return (new_token);
}

t_token *create_token(char *value)
{
	t_token *new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	if (ft_strncmp(value, "<<", 2) == 0)
		new_token->type = HEREDOC;
	else
		new_token->type = APPEND;
	new_token->value = value;
	new_token->next = NULL;
	return (new_token);
}