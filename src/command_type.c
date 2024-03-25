/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_type.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 09:17:37 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/22 09:58:59 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// cc -I../libft -L../libft *.c -lft -lreadline

short	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r' || c == '\0')
		return (1);
	return (0);
}

short	is_pipe(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '|')
			return (1);
		i++;
	}
	return (0);
}
