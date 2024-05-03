/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 19:08:53 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/01 14:29:21 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main()
{
    char *input;

    while (1)
	{
        input = readline("Minishell: ");
        if (!input || ft_strncmp(input, "exit", 4) == 0)
        {
            free(input);
            break;
        }
        else if (ft_strncmp(input, "env", 3) == 0)
        {
            put_env();
            free(input);
            break;
        }
	}
}