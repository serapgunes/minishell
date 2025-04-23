/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_find.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:18:39 by segunes           #+#    #+#             */
/*   Updated: 2025/04/23 12:04:29 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char *pathname(char *command)
{
	const char *path;
	char **path_env;
	const char *join;
	const char *temp;
	int i;
	
	i = 0;
	path = getenv("PATH");//getenv ile ortam değişkenlerine erişiyoruz
	if(!path)
		return (NULL);//erişemezsek null döndürüyoruz
	path_env = ft_split(path, ':');//bu değişkenlerde her konuma bakıyoruz 
	//"/usr/local/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin" örneğin bu şekilde bir ortam değişkeni aldıysak 
	// /usr/local/sbin ilk başta buraya kadar ayırıyoruz
	// /usr/local/bin
	// /usr/bin
	// /bin
	// /usr/sbin
	// /sbin  bu şekilde hepsini alıp : göre ayırdık
	while(path_env[i])
	{
		//daha sonra her bir konum için bakıyoruz diyelimki /usr/bin bu konumda arayacağız kendi komutumuzu arıyoruz diyedlim ki ben "cat" komutunu arıyorum
		//ilk başta /usr/bin/ ile birleştiriyoruz /usr/bin buna "/" eklemiş oluyoruz
		// daha sonra
		//sonra cat ile birleştiriyoruz
		// yani command den gelen komutla onu da biz mainden gönderiyoruz zaten
		temp = ft_strjoin(path_env[i], "/");
		join = ft_strjoin(temp, command);
		//sonra bu birleştirdiğimiz komutun erişilebilir olup olmadığını kontrol ediyoruz
		//eğer erişilebilir ise return ediyoruz
		//access İlk parametre: kontrol etmek istediğin dosya yolu
		//		 İkinci parametre: hangi izinleri kontrol etmek istiyoruz 
		// X_OK => dosyanın çalıştırılabilir olup olmadığını kontrol eder (executable ok demek açılımı çalıştırılabilir mi)
		if (access(join, X_OK) == 0)
			return (join);
		else 
		{
			free((char *)join);
			free((char *)temp);
		}
		i++;
	}
	return (NULL);
}