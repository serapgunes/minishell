/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:32:24 by segunes           #+#    #+#             */
/*   Updated: 2025/06/01 19:14:00 by sakdil           ###   ########.fr       */
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

void	add_token_to_list(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static int	is_word_char(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '|' || c == '<' || c == '>'
		|| c == '\'' || c == '"')
		return (0);
	return (1);
}

static void	handle_single_quote(char *input, int *i, t_token **head)
{
	int		start;
	char	*word;

	start = ++(*i);
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	word = ft_substr(input, start, (*i) - start);
	add_token_to_list(head, create_word_token(word));
	if (input[*i] == '\'')
		(*i)++;
}

static void	handle_double_quote(char *input, int *i, t_token **head)
{
	int		start;
	char	*raw;
	char	*expanded;

	start = ++(*i);
	while (input[*i] && input[*i] != '"')
		(*i)++;
	raw = ft_substr(input, start, (*i) - start);
	expanded = expand_variable(raw);
	free(raw);
	add_token_to_list(head, create_word_token(expanded));
	if (input[*i] == '"')
		(*i)++;
}

static void	handle_word(char *input, int *i, t_token **head)
{
	int		start;
	char	*raw;
	char	*expanded;

	start = *i;
	while (input[*i] && is_word_char(input[*i]))
		(*i)++;
	raw = ft_substr(input, start, (*i) - start);
	expanded = expand_variable(raw);
	free(raw);
	add_token_to_list(head, create_word_token(expanded));
}

static int	handle_redir_operator(char *s, t_token **head)
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
	return (len);
}

static char	*process_quoted(char *s, int *j, char q)
{
	int		start;
	char	*raw;
	char	*expanded;

	start = *j;
	while (s[*j] && s[*j] != q)
		(*j)++;
	raw = ft_substr(s, start, *j - start);
	if (q == '"')
		expanded = expand_variable(raw);
	else
		expanded = ft_strdup(raw);
	free(raw);
	if (s[*j] == q)
		(*j)++;
	return (expanded);
}

static char	*process_unquoted(char *s, int *j)
{
	int		start;
	char	*raw;
	char	*expanded;

	start = *j;
	while (s[*j] && is_word_char(s[*j]))
		(*j)++;
	raw = ft_substr(s, start, *j - start);
	expanded = expand_variable(raw);
	free(raw);
	return (expanded);
}

static int	handle_redir_file(char *s, int *i, t_token **head)
{
	int		j;
	char	*expanded;

	j = 0;
	while (s[j] == ' ' || s[j] == '\t')
		j++;
	if (!s[j])
	{
		*i += j;
		return (j);
	}
	if (s[j] == '\'' || s[j] == '"')
	{
		j++;
		expanded = process_quoted(s, &j, s[j - 1]);
	}
	else
		expanded = process_unquoted(s, &j);
	add_token_to_list(head, create_word_token(expanded));
	*i += j;
	return (j);
}

static int	tokenize_redirection(char *input, t_token **head)
{
	int	op_len;
	int	file_len;

	op_len = handle_redir_operator(input, head);
	file_len = handle_redir_file(input + op_len, &op_len, head);
	return (op_len);
}

static void	handle_special_char(char *input, int *i, t_token **head)
{
	if (input[*i] == '>' || input[*i] == '<')
		*i += tokenize_redirection(input + *i, head);
	else if (input[*i] == '|')
	{
		add_token_to_list(head, create_pipe_token(ft_strdup("|")));
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
		else if (input[i] == '\'')
			handle_single_quote(input, &i, &head);
		else if (input[i] == '"')
			handle_double_quote(input, &i, &head);
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
			handle_special_char(input, &i, &head);
		else if (is_word_char(input[i]))
			handle_word(input, &i, &head);
		else
			i++;
	}
	return (head);
}
