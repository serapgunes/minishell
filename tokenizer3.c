/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:55:20 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/10 14:41:06 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*process_quoted(char *s, int *i, char quote_char)
{
	int	start;
	int	len;

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

char	*process_unquoted(char *s, int *j)
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

char	*normalize_filename(char *str)
{
	if (!str || str[0] == '\0')
	{
		free(str);
		return NULL;
	}
	return (str);
}
