/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:31:48 by segunes           #+#    #+#             */
/*   Updated: 2025/06/03 15:11:23 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_last(t_token *input)
{
	t_token *last;

	last = input;

	while (last && last->next)
		last = last->next;
	if(last->type == APPEND || last->type == REDIR_IN || last->type == REDIR_OUT
		 || last->type == HEREDOC || last->type == PIPE)
		 {
		 	printf("syntax error near unexpected token");
			return;
		 }
}
void ft_parser(t_token *input)
{	
	t_token *start;

	start = input;
	if (!input)
    	return;
	if(input->type == PIPE)
	{
		printf("syntax error near unexpected token");
		return;
	}
	while(input)
	{
		if(input->type == PIPE && input->next->type == PIPE)
		{
			printf("syntax error near unexpected token");
			return;
		}
		if((input->type == APPEND || input->type == REDIR_IN || input->type == REDIR_OUT
		 || input->type == HEREDOC) && (input->next == NULL || input->next->type != WORD))
		 {
		 	printf("syntax error near unexpected token");
			return;
		 }
		input = input->next;		
	}
	ft_last(start);
}

