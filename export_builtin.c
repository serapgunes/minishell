/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:27:34 by sakdil            #+#    #+#             */
/*   Updated: 2025/05/14 13:28:36 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ;

//argümansız export, tüm ortam değişkenlerini alfabetik sırayla listeler.
//Shell'de ayarlanmış tüm değişkenleri ve değerlerini ekrana yazar.

//ÖRNEK : export
//declare -x HOME="/home/user"
//declare -x PATH="/usr/bin:/bin"
//declare -x PWD="/home/user"

// declare : bir değişkenin özelliklerini tanımlamak veya değiştirmek için kullanılır.
// -x : değişkeni ortam değişkeni olarak işaretler.Yani bu değişkeni child processes'e aktarılabilir hale getirir.


//bubble sort : iki bitişik eleman karşılaştırılır ve gerekiyorsa yer değiştirilir.
//her adımda en büyük değer sona atılır. Sona atılan değerler bir daha kontrol edilmeez.


static int is_valid_identifier(const char *name)
{
	int i;

	i = 0;
	if (!name || name[0] == '\0' || (name[0] >= '0' && name[0] <= '9'))
		return (0);
	while (name[i])
	{
		if (!((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z') ||
			(name[i] >= '0' && name[i] <= '9') || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static char *strip_quotes_process(const char *value, char *out, int len)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < len)
	{
		if (value[i] != '"')
		{
			out[j] = value[i];
			j++;
		}
		i++;
	}
	out[j] = '\0';
	return (out);
}

static char *strip_quotes(const char *value)
{
	int quote_count;
	int len;
	char *out;

	quote_count = 0;
	len = 0;
	while (value[len])
	{
		if (value[len] == '"')
			quote_count++;
		len++;
	}
	if (quote_count % 2 != 0)
		return (NULL);
	out = malloc(len - quote_count + 1);
	if (!out)
		return (NULL);
	return (strip_quotes_process(value, out, len));
}

static int find_in_environ(const char *name)
{
	int i, len;
	char *env;

	i = 0;
	len = strlen(name);
	while (environ[i])
	{
		env = environ[i];
		if (strncmp(env, name, len) == 0 && env[len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static int update_environ_extend(const char *name, char *env_val)
{
	char **new_env;
	int count;
	int i;

	(void)name;
	count = 0;
	while (environ[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(env_val);
		return (1);
	}
	i = 0;
	while (i < count)
	{
		new_env[i] = environ[i];
		i++;
	}
	new_env[count] = env_val;
	new_env[count + 1] = NULL;
	free(environ);
	environ = new_env;
	return (0);
}

static int update_environ(const char *name, const char *value)
{
	char *entry;
	char *env_val;
	int idx;

	entry = ft_strjoin(name, "=");
	if (!entry)
		return (1);
	env_val = ft_strjoin(entry, value);
	free(entry);
	if (!env_val)
		return (1);
	idx = find_in_environ(name);
	if (idx >= 0)
	{
		free(environ[idx]);
		environ[idx] = env_val;
		return (0);
	}
	return (update_environ_extend(name, env_val));
}

static void print_export(char **env, int count)
{
    int i;
    char *eq;

    i = 0;
    while (i < count)
    {
        eq = ft_strchr(env[i], '=');
        if (eq)
        {
            if (*(eq + 1) == '\0')
                printf("declare -x %.*s=\"\"\n", (int)(eq - env[i]), env[i]);
            else
                printf("declare -x %.*s=\"%s\"\n",
                       (int)(eq - env[i]), env[i], eq + 1);
        }
        else
            printf("declare -x %s\n", env[i]);
        i++;
    }
}
static void	sort_env(char **env, int count)
{
	int		i;
	int		j;
	char	*key1;
	char	*key2;
	char	*eq1;
	char	*eq2;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			key1 = ft_strdup(env[j]);
			key2 = ft_strdup(env[j + 1]);
			eq1 = ft_strchr(key1, '=');
			eq2 = ft_strchr(key2, '=');
			if (eq1)
				*eq1 = '\0';
			if (eq2)
				*eq2 = '\0';
			if (ft_strcmp(key1, key2) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
			}
			free(key1);
			free(key2);
			j++;
		}
		i++;
	}
}

static int print_sorted_environ(void)
{
	char **copy;
	int count;
	int i;

	count = 0;
	while (environ[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (1);
	i = 0;
	while (i < count)
	{
		copy[i] = environ[i];
		i++;
	}
	copy[count] = NULL;
	sort_env(copy, count);
	print_export(copy, count);
	free(copy);
	return (0);
}

static int builtin_export_handle(char *arg, char *eq, int argc, char **argv, int status, int i)
{
    char *name;
    char *value;
    char *stripped;

    if (eq)
    {
        *eq = '\0';
        name = arg;
        value = eq + 1;
        if (!is_valid_identifier(name)) 
		{
            printf("export: '%s': not a valid identifier\n", name);
            *eq = '=';
            return (builtin_export_process(argc, argv, status + 1, i));
        }
        stripped = strip_quotes(value);
        if (!stripped)
        {
            printf("export: unmatched quote in '%s=%s'\n", name, value);
            *eq = '=';
            return (builtin_export_process(argc, argv, status + 1, i));
        }
        value = stripped;
        if (update_environ(name, value))
        {
            printf("export: failed to set '%s'\n", name);
            status = 1;
        }
        free(stripped);
        *eq = '=';
    }
    else
    {
        name = arg;
        if (!is_valid_identifier(name)) 
		{
            printf("export: '%s': not a valid identifier\n", name);
            return (builtin_export_process(argc, argv, status + 1, i));
        }
        if (find_in_environ(name) < 0)
        {
            char *env_entry = ft_strdup(name);
            if (!env_entry || update_environ_extend(name, env_entry))
            {
                printf("export: failed to export '%s'\n", name);
                status = 1;
            }
        }
    }
    return (builtin_export_process(argc, argv, status, i));
}


int builtin_export_process(int argc, char **argv, int status, int i)
{
	char *arg, *eq;
	while (i < argc)
	{
		arg = argv[i++];
		eq = ft_strchr(arg, '=');
		return (builtin_export_handle(arg, eq, argc, argv, status, i));
	}
	return (status);
}

int builtin_export(int argc, char **argv)
{
	int status;
	int i;

	status = 0;
	i = 1;
	if (argc == 1)
		return (print_sorted_environ());
	return (builtin_export_process(argc, argv, status, i));
}
