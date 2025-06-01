/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 17:51:21 by segunes           #+#    #+#             */
/*   Updated: 2025/06/01 19:13:53 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_free(char *res, char *val, int flag)
{
	char	*tmp;

	tmp = ft_strjoin(res, val);
	if (flag & 1) //res serbest
		free(res);
	if (flag & 2) //val serbest
		free(val);
	return (tmp);
}

char	*ft_strjoin_char(char *res, char c)
{
	char	*tmp;
	int		len;

	len = ft_strlen(res);
	tmp = malloc(len + 2);
	if (!tmp)
		return (NULL);
	ft_strlcpy(tmp, res, len + 1);
	tmp[len] = c;
	tmp[len + 1] = '\0';
	free(res);
	return (tmp);
}

static char	*handle_var(char *str, int *i, char *res)
{
	char	*val;
	char	*var;
	int		j;

	if (str[*i + 1] == '?')
	{
		val = ft_itoa(g_last_status);
		res = ft_strjoin_free(res, val, 3);
		*i += 2;
	}
	else
	{
		j = 1;
		while (str[*i + j] && (ft_isalnum(str[*i + j]) || str[*i + j] == '_'))
			j++;
		var = ft_substr(str, *i + 1, j - 1);
		val = getenv(var);
		if (!val)
			val = ft_strdup("");
		res = ft_strjoin_free(res, ft_strdup(val), 3);
		free(var);
		*i += j;
	}
	return (res);
}

char	*expand_variable(char *str)
{
	char	*res;
	int		i;

	res = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
			res = handle_var(str, &i, res);
		else
			res = ft_strjoin_char(res, str[i++]);
	}
	return (res);
}

t_token	*create_word_token(char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = WORD;
	new_token->value = value;
	new_token->next = NULL;
	return (new_token);
}

t_token	*create_pipe_token(char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = PIPE;
	new_token->value = value;
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
	new_token->value = value;
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
	new_token->value = value;
	new_token->next = NULL;
	return (new_token);
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->value);
		free(head);
		head = tmp;
	}
}
