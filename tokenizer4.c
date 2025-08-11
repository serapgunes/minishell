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

static int	read_redir_target(char *s, int *j, char **arg, char **envp)
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
		}
		else
			piece = process_unquoted(s, j, envp);
		if (append_piece(arg, piece) == -1)
			return (-1);
	}
	return (0);
}

static int	read_and_normalize_redir_arg(char *s, int *j,
					char **norm, char **envp)
{
	char	*arg;

	while (s[*j] == ' ' || s[*j] == '\t')
		(*j)++;
	if (!s[*j])
		return (1);
	arg = ft_strdup("");
	if (!arg)
		return (-1);
	if (read_redir_target(s, j, &arg, envp) == -1)
	{
		free(arg);
		return (-1);
	}
	*norm = normalize_filename(arg);
	free(arg);
	if (!*norm)
	{
		ft_putendl_fd("ambiguous redirect", 2);
		return (-1);
	}
	return (0);
}

int	handle_redir_file(char *s, int *i, t_token **head, char **envp)
{
	int		j;
	int		rc;
	char	*norm;

	j = 0;
	rc = read_and_normalize_redir_arg(s, &j, &norm, envp);
	if (rc == 1)
	{
		*i += j;
		return (j);
	}
	if (rc < 0)
		return (-1);
	add_token_to_list(head, create_word_token(norm));
	free(norm);
	*i += j;
	return (j);
}
