/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 17:52:16 by amakela           #+#    #+#             */
/*   Updated: 2024/05/01 14:20:20 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 #include "../include/minishell.h"

// frees the array of redirs
void	free_str_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

// frees a single process node
void	free_node(t_node *node)
{
	if (node->redirs)
		free_str_array(node->redirs);
	if (node->cmd)
		free(node->cmd);
	free(node);
}

// frees the list of process nodes
void	free_list(t_node **processes)
{
	t_node	*temp;
	if (processes == NULL || *processes == NULL)
	{
		ft_printf(1, "processes == NULL");
		return ;
	}
	while ((*processes)->next != NULL)
	{
		temp = *processes;
		*processes = (*processes)->next;
		free_node(temp);
	}
	free_node(*processes);
	processes = NULL;
}
