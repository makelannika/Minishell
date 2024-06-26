/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:46:20 by amakela           #+#    #+#             */
/*   Updated: 2024/06/07 14:07:41 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	free_node(t_node *node)
{
	if (node->redirs)
	{
		free_str_array(node->redirs);
		node->redirs = NULL;
	}
	if (node->cmd)
		free(node->cmd);
	free(node);
}

void	free_list(t_node **processes)
{
	t_node	*temp;

	if (processes == NULL || *processes == NULL)
		return ;
	while ((*processes)->next != NULL)
	{
		temp = *processes;
		*processes = (*processes)->next;
		free_node(temp);
	}
	free_node(*processes);
	*processes = NULL;
}

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

t_node	*create_node(void)
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
