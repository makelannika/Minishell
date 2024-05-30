#include "../include/minishell.h"

void    *ft_memcpy_array(void *dst, const void *src, size_t sizeof_src)
{
    size_t      i;
    size_t      j;
    char        *d;
    const char  **s;

    d = (char *)dst;
    s = (const char **)src;
    if (!dst || !src)
        return (dst);
    i = 0;
    while (i < sizeof_src)
    {
        j = 0;
        while (s[i][j])
            *d++ = s[i][j++];
        i++;
    }
    *d = '\0';
    return (dst);
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
// echo Michael "Michael" '"$HOME"' "'$HOME'"
// echo Michael "Michael" '"$HOME"' "'Michael'"