/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:32:24 by segunes           #+#    #+#             */
/*   Updated: 2025/05/31 18:04:27 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_token_to_list(t_token **head, t_token *new)
{
    if (!*head)
        *head = new;
    else
    {
        t_token *tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

t_token *tokenize_input(char *input)
{
	int i;
	int start;
	int lenght;
	char *word;
	t_token *head = NULL;
	
	i = 0;
	while(input[i])
	{
		if (input[i] == ' ' || input[i] == '\t')
		{
			i++;
			continue;
		}
		else if(ft_isalpha(input[i]))
		{
			start = i;
			while (ft_isalpha(input[i]) == 1 || input[i] == '.')
				i++;
			lenght = i - start;
			word = ft_substr(input, start, lenght);
			t_token *token = create_word_token(word);
			add_token_to_list(&head, token);
			continue;//koşul içinde i arttığı için diğer koşul görünmeyebilir bununla devam ettiriyoruz
		}
		else if((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))// önce bunlar kontrol edilmeli 
		//eğer önce redirler kontrol edilirse yani < > bu koşul gözden kaçar ve bu koşula girmez
		{
			word = ft_substr(input, i, 2);
			t_token *token = create_token(word);
			add_token_to_list(&head, token);
			i += 2;
			continue;
		}
		else if(input[i] == '>' || input[i] == '<')
		{
			word = ft_substr(input, i, 1);
			t_token *token = create_redir_token(word);
			add_token_to_list(&head, token);			
		}
		else if(input[i] == '|')
		{
			word = ft_substr(input, i, 1);
			t_token *token = create_pipe_token(word);
			add_token_to_list(&head, token);
		}
		i++;
	}
	return(head);
}
