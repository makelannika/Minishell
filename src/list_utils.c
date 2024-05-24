/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:46:20 by amakela           #+#    #+#             */
/*   Updated: 2024/05/23 16:28:21 by amakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// returns the number of nodes/processes
int	get_list_length(t_node *processes)
{
	int	length;

	length = 0;
	while (processes)
	{
		length++;
		processes = processes->next;
	}
	return (length);
}

// adds a node to the back of the process list
void	add_back(t_node **lst, t_node *new)
{
	t_node	*current;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next != NULL)
		current = current->next;
	current->next = new;
}

// creates a node to store information of a single process
t_node	*create_node()
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return (NULL);
	new_node->redirs = NULL;
	new_node->cmd = NULL;
	new_node->builtin = 0;
	new_node->next = NULL;
	return (new_node);
}
