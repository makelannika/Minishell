/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 17:52:16 by amakela           #+#    #+#             */
/*   Updated: 2024/06/04 13:56:23 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// frees an array of strings
void	free_str_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

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

int	close_and_free(t_pipex *data)
{
	close(data->ends[0]);
	close(data->ends[1]);
	close(data->read_end);
	free_first_inits(data);
	if (data->cmd_str)
		free(data->cmd_str);
	if (data->cmd)
		free_str_array(data->cmd);
	if (data->path)
		free(data->path);
	if (data->pids)
		free(data->pids);
	data->cmd_str = NULL;
	data->cmd = NULL;
	data->path = NULL;
	data->pids = NULL;
	return (-1);
}

void	free_parent(t_pipex *data)
{
	if (data->cmd_str)
		free(data->cmd_str);
	if (data->cmd)
		free_str_array(data->cmd);
	if (data->path)
		free(data->path);
	if (data->pids)
		free(data->pids);
	data->cmd_str = NULL;
	data->cmd = NULL;
	data->path = NULL;
	data->pids = NULL;
}
