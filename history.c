/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:46:38 by segunes           #+#    #+#             */
/*   Updated: 2025/04/23 12:48:09 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_history(t_list *history)
{
	t_list	*current;

	current = history;
	while (current)
	{
		printf("%s\n", (char *)current->content);
		current = current->next;
	}
}
void	add_to_history(t_list **history, char *input)
{
	t_list	*new_node;

	new_node = ft_lstnew(ft_strdup(input));
	if (!new_node)
	{
		perror("Failed to allocate memory for history");
		return;
	}
	ft_lstadd_back(history, new_node);
}