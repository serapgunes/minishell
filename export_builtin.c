/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:27:34 by sakdil            #+#    #+#             */
/*   Updated: 2025/05/08 13:15:19 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//argümansız export, tüm ortam değişkenlerini alfabetik sırayla listeler.
//Shell'de ayarlanmış tüm değişkenleri ve değerlerini ekrana yazar.

//ÖRNEK : export
//declare -x HOME="/home/user"
//declare -x PATH="/usr/bin:/bin"
//declare -x PWD="/home/user"

// declare : bir değişkenin özelliklerini tanımlamak veya değiştirmek için kullanılır.
// -x : değişkeni ortam değişkeni olarak işaretler.Yani bu değişkeni child processes'e aktarılabilir hale getirir.

static void	copy_env(char **env, char **env_copy, int env_count)
{
	int	i;

	i = 0;
	while (i < env_count)
	{
		env_copy[i] = env[i];
		i++;
	}
	env_copy[env_count] = NULL;
}

//bubble sort : iki bitişik eleman karşılaştırılır ve gerekiyorsa yer değiştirilir.
//her adımda en büyük değer sona atılır. Sona atılan değerler bir daha kontrol edilmeez.

static void	sort_env(char **env_copy, int env_count) //alfabetik sıralamak için
{
	int		i;
	int		j;
	int		swap; //döngüde değişim yapılıp yapılmadığını kontrol eder.
	char	*temp;

	i = 0;
	while (i < env_count - 1) // neden sonuncu indeks için döngüye girmiyor? İlk turda en büyük eleman sona oturduğundan sonraki turlarda sona yerleşmiş olan elemanı tekrar kontrol etmeye gerek yok.
	{
		j = 0;
		swap = 0;
		while (j < env_count - i - 1) //2 bitişik elemanı karşılaştırmak için
		{
			if (ft_strcmp(env_copy[j], env_copy[j + 1]) > 0)
			{
				temp = env_copy[j];
				env_copy[j] = env_copy[j + 1];
				env_copy[j + 1] = temp;
				swap = 1;
			}
			j++;
		}
		if (!swap) //eğer swap 0 sa zaten sıralı demektir ve direkt çıkılır
			break;
		i++;
	}
}

static void	print_export(char **env_copy, int env_count)
{
	int		i;
	char	*name;
	char	*value;
	char	*temp;

	i = 0;
	while (i < env_count)
	{
		temp = ft_strdup(env_copy[i]);  // Geçici bir kopya oluştur
		if (!temp)
			return; // hata mesajıa gerek var mı?
		name = temp; //o anki ortam değişkeninin tam satırını (örnek PATH=/usr/bin:/bin) tutar.
		value = ft_strchr(temp, '='); // = 'i bulmal gerekiyor. (ilk = bulunduğu adresi döner.)
		if (value)
		{
			*value = '\0';  // = işaretini null a dönüştürerek değişken ismi ve değeri ayrılıypr. Mesela PATH=/usr/bin:/bin : "PATH" ve "usr/bin:/bin" oluyor.
			value++;
			printf("declare -x %s=\"%s\"\n", name, value);
		}
		else // = yoksa demek ki sadece isimden oluşuyor.
		{
			printf("declare -x %s\n", name);
		}
		free(temp);
		i++;
	}
}

int	builtin_export(int argc, char **argv, char **env)
{
	char	**env_copy;
	int		env_count;

	(void)argv;
	env_count = 0;
	if (argc > 1)
	{
		printf("export : no arguments allowed\n");
		return (1);
	}
	while (env[env_count])
		env_count++;
	env_copy = malloc(sizeof(char *) * (env_count + 1));
	if (!env_copy)
		return (1);
	copy_env(env, env_copy, env_count);
	sort_env(env_copy, env_count);
	print_export(env_copy, env_count);
	free(env_copy);
	return (0);
}
