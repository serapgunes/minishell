/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:48:31 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/12 11:44:27 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	read_redir_target_mode(char *s, int *j, char **arg, t_rtok *m)
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
			if (m->has_quote)
				*(m->has_quote) = 1;
		}
		else if (m->expand)
			piece = process_unquoted(s, j, m->envp);
		else
			piece = process_unquoted_raw(s, j);
		if (append_piece(arg, piece) == -1)
			return (-1);
	}
	return (0);
}

static int	read_and_build_redir_arg(char *s, int *j, char **out, t_rtok *ctx)
{
	char	*arg;
	int		rc;

	while (s[*j] == ' ' || s[*j] == '\t')
		(*j)++;
	if (!s[*j])
		return (1);
	if (ctx->has_quote)
		*(ctx->has_quote) = 0;
	arg = ft_strdup("");
	if (!arg)
		return (-1);
	rc = read_redir_target_mode(s, j, &arg, ctx);
	if (rc == -1)
		return (free(arg), -1);
	*out = normalize_filename(arg);
	free(arg);
	if (!*out)
		return (ft_putendl_fd("ambiguous redirect", 2), -1);
	return (0);
}

static int	add_redir_target_word(char *normalized, int in_heredoc,
			int was_quoted, t_token **head)
{
	t_token	*tok;

	tok = create_word_token(normalized);
	if (!tok)
		return (1);
	if (in_heredoc)
		tok->quoted = was_quoted;
	else
		tok->quoted = 0;
	add_token_to_list(head, tok);
	return (0);
}

int	handle_redir_file(char *s, int *i, t_token **head, char **envp)
{
	int		adv;
	int		rc;
	int		q;
	char	*norm;
	t_rtok	ctx;

	adv = 0;
	q = 0;
	norm = NULL;
	ctx.envp = envp;
	ctx.has_quote = &q;
	ctx.expand = !prev_is_heredoc(*head);
	rc = read_and_build_redir_arg(s, &adv, &norm, &ctx);
	if (rc == 1)
	{
		*i += adv;
		return (adv);
	}
	if (rc < 0)
		return (-1);
	if (add_redir_target_word(norm, prev_is_heredoc(*head), q, head))
		return (free(norm), -1);
	free(norm);
	*i += adv;
	return (adv);
}
