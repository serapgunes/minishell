/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 11:06:23 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/06 15:02:39 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_env(char **env)
{
	int		i;
	int		count;
	char	**copy;

	i = 0;
	count = 0;
	while (env[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	while (i < count)
	{
		copy[i] = ft_strdup(env[i]);
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

static int	compare_keys(char *s1, char *s2)
{
	char	*k1;
	char	*k2;
	int		res;

	k1 = ft_substr(s1, 0, ft_strchr(s1, '=') - s1);
	k2 = ft_substr(s2, 0, ft_strchr(s2, '=') - s2);
	if (!k1 || !k2)
	{
		free(k1);
		free(k2);
		return (0);
	}
	res = ft_strcmp(k1, k2);
	free(k1);
	free(k2);
	return (res);
}

static void	sort_env(char **copy)
{
	int		n;
	int		i;
	int		j;
	char	*tmp;

	n = 0;
	i = 0;
	while (copy[n])
		n++;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (compare_keys(copy[j], copy[j + 1]) > 0)
			{
				tmp = copy[j];
				copy[j] = copy[j + 1];
				copy[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_sorted_env(char **env)
{
	char	**copy;
	int		i;
	char	*tmp;

	i = 0;
	copy = copy_env(env);
	if (!copy)
		return;
	sort_env(copy);
	while (copy[i])
	{
		if (copy[i][0] == '\0')
		{
			i++;
			continue;
		}
		tmp = ft_strchr(copy[i], '=');
		if (tmp)
			printf("declare -x %.*s=\"%s\"\n", (int)(tmp - copy[i]), copy[i], tmp + 1);
		else
			printf("declare -x %s\n", copy[i]);
		i++;
	}
	free(copy);
}

void	print_invalid_identifier(char *arg)
{
	char	*msg;
	char	*temp;

	msg = ft_strjoin("export: '", arg);
	if (!msg)
		return;
	temp = ft_strjoin(msg, "': not a valid identifier\n");
	free(msg);
	if (!temp)
		return;
	ft_putstr_fd(temp, 2);
	free(temp);
}
