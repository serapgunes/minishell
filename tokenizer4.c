/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:48:31 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/09 21:58:14 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}

static int	append_piece(char **arg, char *piece)
{
	char	*joined;

	if (!piece)
	{
		free(*arg);
		ft_putendl_fd("redirect syntax error", 2);
		return (-1);
	}
	joined = ft_charjoin_free(*arg, piece, 3);
	if (!joined)
	{
		*arg = NULL;
		return (-1);
	}
	*arg = joined;
	return (0);
}

static t_token	*last_token(t_token *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

static char	*process_unquoted_raw(char *s, int *j)
{
	int		start;

	start = *j;
	while (s[*j] && !is_redir_separator(s[*j])
		&& s[*j] != '\'' && s[*j] != '"')
		(*j)++;
	return (ft_substr(s, start, *j - start));
}

static int	read_redir_target_mode(char *s, int *j, char **arg,
				char **envp, int *has_quote, int expand)
{
	char	quote;
	char	*piece;

	while (s[*j] && !is_redir_separator(s[*j]))
	{
		if (s[*j] == '\'' || s[*j] == '"')
		{
			quote = s[*j];
			(*j)++;
			piece = process_quoted(s, j, quote);
			*has_quote = 1;
		}
		else if (expand)
			piece = process_unquoted(s, j, envp);
		else
			piece = process_unquoted_raw(s, j);
		if (append_piece(arg, piece) == -1)
			return (-1);
	}
	return (0);
}

static int	read_and_build_redir_arg(char *s, int *j, char **out,
				char **envp, int *was_quoted, int expand)
{
	char	*arg;
	int		rc;

	while (s[*j] == ' ' || s[*j] == '\t')
		(*j)++;
	if (!s[*j])
		return (1);
	*was_quoted = 0;
	arg = ft_strdup("");
	if (!arg)
		return (-1);
	rc = read_redir_target_mode(s, j, &arg, envp, was_quoted, expand);
	if (rc == -1)
		return (free(arg), -1);
	*out = normalize_filename(arg);
	free(arg);
	if (!*out)
		return (ft_putendl_fd("ambiguous redirect", 2), -1);
	return (0);
}

int	handle_redir_file(char *s, int *i, t_token **head, char **envp)
{
	int			j;
	int			rc;
	int			was_q;
	int			is_heredoc;
	char		*norm;
	t_token		*op;
	t_token		*w;

	j = 0;
	was_q = 0;
	norm = NULL;
	op = last_token(*head);
	is_heredoc = (op && op->type == HEREDOC);
	rc = read_and_build_redir_arg(s, &j, &norm, envp, &was_q, !is_heredoc);
	if (rc == 1)
		return (*i += j, j);
	if (rc < 0)
		return (-1);
	w = create_word_token(norm);
	if (!w)
		return (free(norm), -1);
	w->quoted = is_heredoc ? was_q : 0;
	add_token_to_list(head, w);
	free(norm);
	*i += j;
	return (j);
}
