/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 11:06:14 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/10 23:20:15 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_in_environ(const char *name, char **env)
{
	int	i;
	int	len;

	if (!env || !name)
		return (-1);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 &&
			(env[i][len] == '=' || env[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

int	extend_env(char ***envp, char *new_entry)
{
	char	**old;
	char	**new;
	int		cnt;
	int		j;

	old = *envp;
	cnt = 0;
	j = 0;
	while (old[cnt])
		cnt++;
	new = malloc(sizeof(char *) * (cnt + 2));
	if (!new)
		return (1);
	while (j < cnt)
	{
		new[j] = old[j];
		j++;
	}
	new[j++] = new_entry;
	new[j] = NULL;
	*envp = new;
	free (old);
	return (0);
}

static int	set_env_var_update(char *name, char *value, char **envp, int idx)
{
	char	*tmp;
	char	*new_var;

	free(envp[idx]);
	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (1);
	new_var = ft_strjoin(tmp, value);
	free(tmp);
	if (!new_var)
		return (1);
	envp[idx] = new_var;
	return (0);
}

static int	set_env_var_add(char *name, char *value, char ***envp)
{
	char	*tmp;
	char	*new_var;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (1);
	new_var = ft_strjoin(tmp, value);
	free(tmp);
	if (!new_var)
		return (1);
	if (extend_env(envp, new_var))
		return (free(new_var), 1);
	return (0);
}

int	set_env_var(char *arg, char ***envp)
{
	char	*eq;
	char	*name;
	char	*value;
	int		idx;
	int		ret;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (1);
	name = ft_substr(arg, 0, eq - arg);
	value = ft_strdup(eq + 1);
	if (!name || !value)
	{
		free(name);
		free(value);
		return (1);
	}
	idx = find_in_environ(name, *envp);
	if (idx >= 0)
		ret = set_env_var_update(name, value, *envp, idx);
	else
		ret = set_env_var_add(name, value, envp);
	free(name);
	free(value);
	return (ret);
}
