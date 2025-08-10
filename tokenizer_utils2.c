/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 10:58:23 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/10 13:43:46 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_var_utils(char *res, char *str, int *i, int j)
{
	char	*var;
	char	*val;
	char	*rem;	

	var = ft_substr(str, *i + 1, j - 1);
	if (!var)
		return (NULL);
	val = getenv(var);
	if (val)
		res = ft_charjoin_free(res, ft_strdup(val), 3);
	else
	{
		rem = ft_substr(str, *i + j, ft_strlen(str) - (*i + j));
		res = ft_charjoin_free(res, rem, 3);
		*i = ft_strlen(str);
		free(var);
		return (res);
	}
	free(var);
	*i += j;
	return (res);
}

static char	*handle_var(char *str, int *i, char *res)
{
	int		j;
	char	*code;
	
	j = 1;
	if (str[*i + 1] == '?')
	{
		code = ft_itoa(ft_exit_code(-1));
		res = ft_charjoin_free(res, code, 3);
		*i += 2;
		return (res);
	}
	if (is_digit(str[*i + 1]))
		j = 2;
	else if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
	{
		while (str[*i + j] && (ft_isalnum(str[*i + j]) || str[*i + j] == '_'))
			j++;
	}
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
	int	idx;

	idx = 0;
	while (str[idx])
	{
		if (str[idx] == '$')
			res = handle_var(str, &idx, res);
		else
			res = ft_charjoin(res, str[idx++]);
		if (!res)
			return (NULL);
	}
	return (res);
}

char	*expand_variable(char *str)
{
	char	*res;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	return (expand_variable_loop(str, res));
}
