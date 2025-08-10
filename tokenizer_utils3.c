/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:59:29 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/10 14:39:43 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_word_token(char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = WORD;
	new_token->value = ft_strdup(value);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->next = NULL;
	return (new_token);
}

// t_token *create_pipe_token(char *value)
// {
// 	t_token *new_token;

// 	new_token = malloc(sizeof(t_token));
// 	if (!new_token)
// 		return (NULL);
// 	new_token->type = PIPE;
// 	new_token->value = ft_strdup(value);
// 	if (!new_token->value)
// 	{
// 		free(new_token);
// 		return (NULL);
// 	}
// 	new_token->next = NULL;
// 	return (new_token);
// }

t_token	*create_pipe_token(char *value)
{
	t_token	*new_token;
	char	*dup_value;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);

	dup_value = ft_strdup(value);
	if (!dup_value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = PIPE;
	new_token->value = dup_value;
	new_token->next = NULL;
	return (new_token);
}

t_token	*create_redir_token(char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	if (value[0] == '<')
		new_token->type = REDIR_IN;
	else
		new_token->type = REDIR_OUT;
	new_token->value = ft_strdup(value);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->next = NULL;
	return (new_token);
}

t_token	*create_token(char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	if (ft_strncmp(value, "<<", 2) == 0)
		new_token->type = HEREDOC;
	else
		new_token->type = APPEND;
	new_token->value = ft_strdup(value);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->next = NULL;
	return (new_token);
}
