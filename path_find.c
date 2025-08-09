/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_find.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:25:32 by segunes           #+#    #+#             */
/*   Updated: 2025/08/09 21:35:53 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static char	*try_join_exec(const char *path, char *command)
{
	char	*temp;
	char	*join;
	char	*ret;

	temp = ft_strjoin(path, "/");  // PATH/command şeklinde birleştir
	if (!temp)
		return (NULL);
	join = ft_strjoin(temp, command);
	free(temp);
	if (!join)
		return (NULL);
	ret = NULL;
	if (access(join, X_OK) == 0)
		ret = ft_strdup(join);  // Kopya oluştur
	free(join);
	return (ret);
}

static char	*search_path(char **path_env, char *command)
{
	int		i;
	char	*res;

	i = 0;
	res = NULL;
	while (path_env[i] && !res)
	{
		res = try_join_exec(path_env[i], command);
		i++;
	}
	return (res);
}

char	*find_path(char *command)
{
	const char	*path;
	char		**path_env;
	char		*result;

	if (command[0] == '/' || command[0] == '.') // Önce komutun mutlak yol olup olmadığını kontrol et
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	path = getenv("PATH");   // PATH ortam değişkenini al
	if (!path)
		return (NULL);
	path_env = ft_split(path, ':');
	if (!path_env)
		return (NULL);
	result = search_path(path_env, command);
	free_string_array(path_env);  // path_env dizisini tamamen temizle
	return (result);
}
