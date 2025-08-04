/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:58:23 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/04 20:08:22 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_var_utils(char *res, char *str, int *i, int j)
{
	char	*var;
	char	*val;

	var = ft_substr(str, *i + 1, j - 1);
	if (!var)
		return (NULL);
	val = getenv(var);
	free(var);
	if (val)
		res = ft_charjoin_free(res, ft_strdup(val), 3);
	*i += j;
	return (res);
}

static char	*handle_var(char *str, int *i, char *res)
{
	char	*val;
	int		j;

	j = 1;
	if (str[*i + 1] == '?')
	{
		val = ft_itoa(ft_exit_code(-1));
		if (!val)
			return (NULL);
		res = ft_charjoin_free(res, val, 3);
		*i += 2;
		return (res);
	}
	while (str[*i + j] && (ft_isalnum(str[*i + j]) || str[*i + j] == '_'))
		j++;
	if (j == 1)
	{
		res = ft_charjoin(res, '$');
		(*i)++;
		return (res);
	}
	return (handle_var_utils(res, str, i, j));
}

static char	*expand_variable_loop(char *str, char *res)
{
	int		i;
	char	*tmp;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			tmp = handle_var(str, &i, res);
			if (!tmp)
				return (NULL);
			res = tmp;
		}
		else
		{
			tmp = ft_charjoin(res, str[i++]);
			if (!tmp)
				return (NULL);
			res = tmp;
		}
	}
	return (res);
}

char	*expand_variable(char *str)
{
	char	*res;
	char	*tmp;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	tmp = expand_variable_loop(str, res);
	return (tmp);
}
