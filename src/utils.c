/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:06:02 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/03 10:21:27 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

short	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r' || c == '\0')
		return (1);
	return (0);
}

int	len_to_first_space(char *s)
{
	int	i;

	i = 0;
	while (s[i] && !is_space(s[i]))
		i++;
	return (i);
}

int	skip_spaces(char *s)
{
	int	i;

	i = 0;
	while (s[i] && is_space(s[i]))
		i++;
	return (i);
}

int	find_char(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (-1);
}
