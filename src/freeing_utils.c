/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 17:52:16 by amakela           #+#    #+#             */
/*   Updated: 2024/05/17 13:53:03 by amakela          ###   ########.fr       */
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
static void	free_node(t_node *node)
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
		return ;
	while ((*processes)->next != NULL)
	{
		temp = *processes;
		*processes = (*processes)->next;
		free_node(temp);
	}
	free_node(*processes);
	processes = NULL;
}

// frees pipex data and closes fds
int	close_and_free(t_pipex *data)
{
	int	i;

	i = 0;
	if (data->ends[0] != -1)
		close(data->ends[0]);
	if (data->ends[1] != -1)
		close(data->ends[1]);
	if (data->read_end != -1)
		close(data->read_end);
	if (data->env)
		free_str_array(data->env);
	if (data->paths)
		free_str_array(data->paths);
	if (data->cmd_str)
		free(data->cmd_str);
	if (data->cmd)
		free_str_array(data->cmd);
	if (data->path)
		free(data->path);
	if (data->pids)
		free(data->pids);
	return (-1);
}
