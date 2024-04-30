/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_strjoin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 09:41:07 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/29 10:13:03 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*join_and_free_sec(char *s1, char *s2)
{
	char	*str;

	str = ft_strjoin(s1, s2);
	free(s2);
	return (str);
}

char	*f_strjoin(char *s1, char *s2)
{
	char	*str;
	size_t	l1;
	size_t	l2;
	size_t	i;

	if (!s1)
		return (ft_strdup(s2));
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	str = (char *) ft_calloc((l1 + l2 + 1), sizeof(char));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < l1)
	{
		str[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < l2)
	{
		str[i + l1] = s2[i];
		i++;
	}
	return (free(s1), str);
}

/*int main()
{
	printf("%s\n", ft_strjoin("ok,", " funziona"));
	return 0;
}*/
