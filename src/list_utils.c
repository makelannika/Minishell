/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:46:20 by amakela           #+#    #+#             */
/*   Updated: 2024/06/05 12:27:48 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	remove_key(char **cmd, int start, int end)
{
	int	i;

	i = 0;
	while ((*cmd)[end + i])
	{
		(*cmd)[start + i] = (*cmd)[end + i];
		i++;
	}
	(*cmd)[start + i] = '\0';
}

void	remove_not_expandable(char **cmd, int key_start)
{
	int	i;

	i = 0;
	while ((*cmd)[key_start + i])
	{
		if ((*cmd)[key_start + i] == '$' || (*cmd)[key_start + i] == ' ')
			remove_key(cmd, key_start, key_start - i);
		i++;
	}
	(*cmd)[key_start + i] = '\0';
}
