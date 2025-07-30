/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:55:20 by sakdil            #+#    #+#             */
/*   Updated: 2025/07/30 18:37:49 by segunes          ###   ########.fr       */
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

static char *process_quoted(char *s, int *j, char q)
{
	int start;
	char *raw;
	char *expanded;

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
	char *expanded;

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

	expanded = normalize_filename(expanded);
	if (!expanded) // boş dosya adı varsa
	{
		fprintf(stderr, "minishell: ambiguous redirect\n");
		// bu ksım da kaldım exit_status = 1;
		return (-1); // işlemi durdur
	}
	add_token_to_list(head, create_word_token(expanded));
	*i += j;
	return (j);
}
