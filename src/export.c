/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakela <amakela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 16:33:21 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/22 11:10:31 by amakela          ###   ########.fr       */
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
    if (!(ft_isalpha(str[0]) && str[0] != '_'))
        return (0);
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalpha(str[i]) && str[i] != '_' && !ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

void    print_export(char *str, int fd_out)
{
    char    *tmp;
    int     i;

    i = 0;
    tmp = ft_strchr(str, '=');
    if (!tmp) //prints string without =
        ft_printf(fd_out, "declare -x %s\n", str);
    else if (*(tmp + 1) == '\0') //prints strings with = at the end
        ft_printf(fd_out, "declare -x %s\"\"\n", str);
    else
        printf("declare -x %.*s\"%s\"\n", (int)(tmp - str + 1), str, tmp + 1);
}

char    **do_export(t_pipex *data, char **env, char **cmd, int fd_out)
{
    (void)data;
	int     i;
	
	i = 0;
	if (!cmd[1])
	{
        sort_strings(env);
        while (env[i])
            print_export(env[i++], fd_out);
    }
	else
    {
        i = 1;
        while (cmd[i])
        {
            if (!check_key(cmd[i]))
            {
                ft_printf(2, "export: `%s': not a valid identifier\n", cmd[i++]);
                continue;
            }
		    env = putstr_in_array(env, cmd[i++]);
        }
        // printf("AFTER EXPORT_________\n");
        // for (int p = 0; env[p]; p++)
        //     printf("%s\n", env[p]);
        // printf("\n\n\n");
    }
    return (env);
}
// int main()
// {
//     char **exports = malloc(sizeof(char *) * 8);
//     char **env = malloc(sizeof(char *) * 9);

//     exports[0] = ft_strdup("export");
//     exports[1] = ft_strdup("1cat=10");
//     exports[2] = ft_strdup("cat=9");
//     exports[3] = ft_strdup("cat");
//     exports[4] = ft_strdup("cat=");
//     exports[5] = NULL;

//     char **str= malloc(sizeof(char *) * 9);
//     str[0] = "export";
//     str[2] = NULL;

//     env[0] = ft_strdup("dog=6");
//     env[1] = ft_strdup("dog=7");
//     env[2] = ft_strdup("dog=7=234sdf");
//     env[3] = NULL;
//     do_export(env, exports, 2);
//     printf("____________________\n");
//     do_export(env, str ,2);
// }
//TODO: update key that aleady exists
//TODO: figure out exit or return
