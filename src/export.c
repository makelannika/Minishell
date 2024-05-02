/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:33:21 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/02 18:48:48 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_in_alpha(char**env)
{
	int	i;
	char *tmp;
	int array_len;

	i = 0;
	array_len = array_len(env);
	while (env[i])
	{
		
	}
}
//FIXME: DO WE PUT INTO ALPH OR NOT?
void do_export(char **env, char *cmd)
{
	int	i;
	
	i = 0;
	if ((ft_strncmp(cmd, "export", 7) == 0) || (ft_strncmp(cmd, "EXPORT", 7) == 0))
		execute_export(env)
	else
		putstr_in_array(env, cmd);
}