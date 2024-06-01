/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linhnguy <linhnguy@hive.student.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:38:41 by linhnguy          #+#    #+#             */
/*   Updated: 2024/05/17 20:40:37 by linhnguy         ###   ########.fr       */
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

void	remove_string(char **env, int index)
{
	free(env[index]);
	while (env[index + 1])
	{
		env[index] = env[index + 1];
		index++;
	}
	env[index] = NULL;
}

_Bool	ft_isdigit_str(char *str)
{
	while (*str)
	{
		if (*str < '0' && *str > '9')
			return (0);
		str++;
	}
	return (1);
}

void	set_error_and_print(t_pipex *data, int error, char *msg)
{
	data->curr_exitcode = error;
	ft_printf(2, "%s\n", msg);
}

void	print_error_and_exit(my_printffd my_printf, char *cmd0, char *cmd1, int curr_exitcode) //use only for exit builtin
{
	if	(my_printf && cmd0 && cmd1)
		my_printf(2, "%s %s: numeric argument required\n", cmd0, cmd1);
	exit(curr_exitcode);
}

void	free_and_exit(t_pipex *data, int curr_exitcode) //use only for exit builtin
{
	close_and_free(data);
	exit(curr_exitcode);
}

char	*set_error_return(t_pipex *data, int error, char *msg)
{
	data->curr_exitcode = error;
	ft_printf(2, "%s\n", msg);
	return (NULL);
}
void	*ft_memcat(void *dst, const void *src)
{
	char		*d;
	const char	*s;

	d = (char *)dst;
	s = (const char *)src;
	if (!dst || !src)
		return (dst);
	while (*d)
		d++;
	while (*s != '\0')
		*d++ = *s++;
	*d = '\0';
	return (dst);
}


// void    *ft_memcpy_array(void *dst, const void *src, size_t sizeof_src)
// {
//     size_t      i;
//     size_t      j;
//     char        *d;
//     const char  **s;

//     d = (char *)dst;
//     s = (const char **)src;
//     if (!dst || !src)
//         return (dst);
//     i = 0;
//     while (i < sizeof_src)
//     {
//         j = 0;
//         while (s[i][j])
//             *d++ = s[i][j++];
//         i++;
//     }
//     *d = '\0';
//     return (dst);
// }