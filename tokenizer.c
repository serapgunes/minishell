/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:32:24 by segunes           #+#    #+#             */
/*   Updated: 2025/08/10 14:40:25 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void print_tokens(t_token *head)
// {
// 	t_token *tmp = head;

// 	while (tmp)
// 	{
// 		printf("TOKEN TYPE: %u\tVALUE: %s\n", tmp->type, tmp->value);
// 		tmp = tmp->next;
// 	}
// }// type yazdırmak için kontrol

int	is_word_char(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '|' || c == '<' || c == '>' || c == '\'' || c == '"')
		return (0);
	return (1);
}

static int	tokenize_redirection(char *input, t_token **head)
{
	int	op_len;

	op_len = handle_redir_operator(input, head);
	handle_redir_file(input + op_len, &op_len, head);
	return (op_len);
}

static void	handle_special_char(char *input, int *i, t_token **head)
{
	t_token	*token;

	if (input[*i] == '>' || input[*i] == '<')
		*i += tokenize_redirection(input + *i, head);
	else if (input[*i] == '|')
	{
		token = create_pipe_token("|");
		if (token) // token NULL değilse
			add_token_to_list(head, token);
		(*i)++;
	}
	else
		(*i)++;
}

t_token	*tokenize_input(char *input)
{
	int		i;
	t_token	*head;

	i = 0;
	head = NULL;
	while (input[i])
	{
		if (input[i] == ' ' || input[i] == '\t')
			i++;
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
			handle_special_char(input, &i, &head);
		else
		{
			char *arg = collect_argument(input, &i);
			if (!arg)
			{
				free_token_list(head);
				return (NULL);
			}
			char *tmp = ft_strdup(arg);
			add_token_to_list(&head, create_word_token(tmp));
			free(tmp);
			free(arg);
		}
	}
	return (head);
}

int	handle_redir_operator(char *s, t_token **head)
{
	char	*op;
	int		len;

	if (s[0] == '>' && s[1] == '>')
	{
		op = ft_substr(s, 0, 2);
		len = 2;
	}
	else if (s[0] == '<' && s[1] == '<')
	{
		op = ft_substr(s, 0, 2);
		len = 2;
	}
	else
	{
		op = ft_substr(s, 0, 1);
		len = 1;
	}
	if (len == 2)
		add_token_to_list(head, create_token(op));
	else
		add_token_to_list(head, create_redir_token(op));
	free(op);
	return (len);
}
