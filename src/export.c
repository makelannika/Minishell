/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:33:21 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/11 18:12:37 by linhnguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void static swap_strings(char **a, char **b)
{
    char *temp = *a;
    *a = *b;
    *b = temp;
}

void sort_strings(char **arr)
{
    int swapped;
   	int i;
	int size;

	size = array_len(arr);
	swapped = 1;
    while (swapped)
	{
        swapped = 0;
        i = 0;
        while (i < size - 1)
		{
            if (ft_strncmp(arr[i], arr[i + 1], 400) > 0)
			{
                swap_strings(&arr[i], &arr[i + 1]);
                swapped = 1;
            }
            i++;
        }
    }
}
void    putstr_in_array(char ***env, char *str)
{
    char **tmp;
	int i;

    i = 0;
    tmp = *env;
    *env = malloc(sizeof(char*) *(array_len(*env) + 1));
    while (tmp[i])
    {
        (*env)[i] = ft_strdup(tmp[i]);
        free(tmp[i]);
        // if (!*env)
        //     FREE and PRINT error ft
		i++;
    }
    (*env)[i++] = ft_strdup(str);
    (*env)[i] = NULL;
    free(tmp);
}
//TODO: need to figure out empty string thingy and if first digits is a number or _
//TODO: figure out exit or return
//TODO: multiple variables
void    do_export(char **env, char **cmd)
{
	int     i;
	char    *tmp;

	i = 0;
	if (!cmd[1])
	{
        sort_strings(env);
        while (env[i])
        {
            tmp = ft_strchr(env[i], '=');
            if (!tmp)
                ft_printf(1, "declare -x %s\n", env[i]);
            else if (*(tmp + 1) == '\0')
                ft_printf(1, "declare -x %s\'\'\n", env[i]);
            else
                ft_printf(1, "declare -x %s\n", env[i]);
            i++;
        }
    }
	else
		putstr_in_array(&env, cmd[1]);
}
int main()
{
    char **s = malloc(4 * sizeof(char*));
    s[0] = ft_strdup("that=werwe");
    s[1] = ft_strdup("AFD");
    s[2] = ft_strdup("werre=34534");
    s[3] = NULL;

    char **t = malloc(4 * sizeof(char*));
    t[0] = ft_strdup("export");
    t[1] = NULL;
    do_export(s,t);
    // for(int i = 0; s[i]; i++)
    //     ft_printf(1, "%s\n", s[i]);
}
