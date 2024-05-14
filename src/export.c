/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:33:21 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/14 14:30:05 by amakela          ###   ########.fr       */
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

char    **putstr_in_array(char **env, char *str)
{
    char **tmp;
	int i;

    i = 0;
    tmp = env;
    env = malloc(sizeof(char*) *(array_len(env) + 2));
    while (tmp[i])
    {
        // ft_strchr(tmp[i], '=');
        env[i] = ft_strdup(tmp[i]);
        free(tmp[i]);
        // if (!*env)
        //     FREE and PRINT error ft
		i++;
    }
    env[i++] = ft_strdup(str);
    env[i] = NULL;
    free(tmp);
    return (env);
}
_Bool check_key(char *str)
{
    int i;

    i = 1;
    if (!(ft_isalpha(str[0]) || str[0] != '_'))
        return (false);
    while (str[i] != '=')
    {
        if (!ft_isalpha(str[i]) || str[i] != '_' || !ft_isdigit(str[i]))
            return (false);
        i++;
    }
    return (true);
}

void    print_export(char *str)
{
    char    *tmp;
    int     i;

    i = 0;
    tmp = NULL;
    if (!check_key(str))
        //print error message and return
    tmp = ft_strchr(str, '=');
    if (!tmp) //prints string without =
        ft_printf(1, "declare -x %s\n", str);
    else if (*(tmp + 1) == '\0') //prints strings with = at the end
        ft_printf(1, "declare -x %s\'\'\n", str);
    else
        printf("declare -x %.*s\"%s\"\n", (int)(tmp - str + 1), str, tmp + 1);
}

char    **do_export(char **env, char **cmd)
{
	int     i;
	
	i = 0;
	if (!cmd[1])
	{
        ft_printf(2, "here\n");
        sort_strings(env);
        for(int i =0; env[i]; i++)
        ft_printf(2, "%s\n", env[i]);
        while (env[i])
            print_export(env[i++]);
    }
	else
    {
        i = 1;
        while (cmd[i])
		    env = putstr_in_array(env, cmd[i++]);
    }
    return (env);
}
// int main()
// {
//     char **array = malloc(sizeof(char *) * 8);
//     char **env = malloc(sizeof(char *) * 9);

//     array[0] = ft_strdup("export");
//     array[1] = ft_strdup("cat=10");
//     array[2] = ft_strdup("pipe=9");
//     array[3] = ft_strdup("ppopoe");
//     array[4] = ft_strdup("pipsdfe=");
//     array[5] = NULL;

//     char **str= malloc(sizeof(char *) * 9);
//     str[0] = "export";
//     str[1] = NULL;


//     env[0] = ft_strdup("cat=6");
//     env[1] = ft_strdup("dog=7");
//     env[2] = ft_strdup("dog=7=234sdf");
//     env[3] = NULL;
//     env = do_export(env, array);
//     do_export(env, str);
    
// }
//TODO: need to figure out empty string thingy and if first digits is a number or _
//TODO: figure out exit or return
//TODO: multiple variables
