/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:38:41 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/17 14:45:15 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	array_len(char **a)
{
	int i;

	i = 0;
	while(a[i])
		i++;
	return (i);
}

void	remove_string(char **src, int index)
{
	free(src[index]);
	while (src[index + 1])
	{
		src[index] = src[index + 1];
		index++;
	}
	src[index] = NULL;
}

_Bool	ft_isdigit_str(char *str)
{
	while (*str)
	{
		if (*str <= 48 && *str >= 57)
			return (0);
		str++;
	}
	return (1);
}