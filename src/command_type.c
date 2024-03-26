/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_type.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 09:17:37 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/26 12:04:32 by stescaro         ###   ########.fr       */
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

static	short	is_string(char *line)
{
	int	i;

	i = 0;
	while (line[i] && is_space(line[i]))
		i++;
	if (line[i])
		return (1);
	return (0);
}

short	is_pipe(char *line)
{
	int	i;

	i = 1;
	while (line[i] && line[i + 1] && line[i + 2])
	{
		if (line[i] == '|' && !is_space(line[i - 1]) && !is_space(line[i + 1])
			&& is_string(&line[i + 2]))
			return (1);
		i++;
	}
	return (0);
}

void	skip_printf(char *line)
{
	int	i;
	int	j;
	int	t;

	i = 0;
	while (line[i])
	{
		j = i;
		t = 0;
		while (line[i + t] && !is_space(line[i + t]))
			t++;
		i += t;
		write(1, &line[j], t);
		while (line[i] && is_space(line[i]))
			i++;
		if (line[i])
			write (1, " ", 1);
	}
}
