/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 20:06:34 by sakdil            #+#    #+#             */
/*   Updated: 2025/05/18 20:22:16 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int is_digit(int c)
{
    return (c >= '0' && c <= '9');
}

static int is_numeric(const char *str)
{
    int i = 0;
    if (str[0] == '-' || str[0] == '+')
        i++;
    while (str[i])
    {
        if (!is_digit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

static int is_too_large( char *str)
{
    int len;
    
    len = ft_strlen(str);
    if (len > 19)
        return (1);
    if (len == 19 && ft_strcmp(str, "9223372036854775807") > 0)
        return (1);
    return (0);
}

int builtin_exit(int argc, char **argv)
{
    int exit_code;
    
    exit_code = 0;
	printf("exit\n");
    if (argc > 2)
    {
        printf("exit: too many arguments\n");
        return (1);
    }
    if (argc == 2)
    {
        if (!is_numeric(argv[1]) || is_too_large(argv[1]))
        {
            printf("exit: %s: numeric argument required\n", argv[1]);
            exit(2);
        }
        exit_code = (int)(ft_atoi(argv[1]) % 256);
    }
    exit(exit_code);
    return (0);
}
