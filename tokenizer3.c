/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:55:20 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/05 16:03:22 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_token_to_list(t_token **head, t_token *new)
{
	t_token *tmp;

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

int handle_redir_operator(char *s, t_token **head)
{
	char *op;
	int len;

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

char *process_quoted(char *s, int *i, char quote_char)
{
	int start;
	int len;

	start = *i;
	len = 0;
	while (s[*i] && s[*i] != quote_char)
	{
		(*i)++;
		len++;
	}
	if (s[*i] == quote_char)
		(*i)++;
	return (ft_substr(s, start, len));
}

static char *process_unquoted(char *s, int *j)
{
	int start;
	char *raw;
	char *expanded;

	start = *j;
	while (s[*j] && is_word_char(s[*j]))
		(*j)++;
	raw = ft_substr(s, start, *j - start);
	expanded = expand_variable(raw);
	free(raw);
	return (expanded);
}

char *normalize_filename(char *str)
{
	if (!str)
		return (NULL);
	if (str[0] == '\0')
	{
		free(str);
		return (NULL);
	}
	return (str);
}

int handle_redir_file(char *s, int *i, t_token **head)
{
	int j;
	char *arg;
	char *piece;
	char quote;

	j = 0;
	arg = ft_strdup("");
	while (s[j] == ' ' || s[j] == '\t')
		j++;
	if (!s[j])
	{
		*i += j;
		return (j);
	}
	while (s[j] && s[j] != ' ' && s[j] != '\t' &&
		   s[j] != '|' && s[j] != '<' && s[j] != '>')
	{
		if (s[j] == '\'' || s[j] == '"')
		{
			quote = s[j];
			j++;
			piece = process_quoted(s, &j, quote);
		}
		else
			piece = process_unquoted(s, &j);
		if (!piece)
		{
			free(arg);
			fprintf(stderr, "redirect syntax error\n");
			return (-1);
		}
		arg = ft_charjoin_free(arg, piece, 3);
	}
	arg = normalize_filename(arg);
	if (!arg)
	{
		fprintf(stderr, "ambiguous redirect\n");
		return (-1);
	}
	add_token_to_list(head, create_word_token(arg));
	*i += j;
	return (j);
}
