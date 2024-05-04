/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 09:55:24 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/26 16:07:10 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	is_awk(char *line, int i)
{
	i -= 2;
	if (i < 3)
		return (0);
	while (i > 0 && line[i] != '&' && line[i] != '|' && line[i] != ';'
		&& line[i] != '(')
		i--;
	if (i > 0)
		i++;
	while (line[i] && is_space(line[i]))
		i++;
	if ((!strncmp(&line[i], "awk", 3) && is_space(line[i + 3]))
		|| ((!strncmp(&line[i], "\"awk\"", 5)
				|| !strncmp(&line[i], "\'awk\'", 5)) && is_space(line[i + 5])))
		return (1);
	return (0);
}

short	is_limiter_or_filename(int i, char *l, char c)
{
	int	j;
	int	k;

	j = i;
	if (is_awk(l, i))
		return (-1);
	while (j >= 0 && l[j] != c)
		j--;
	j--;
	while (j >= 0 && is_space(l[j]))
		j--;
	if (j >= 0 && (l[j] != '<' || l[j] != '>'))
		return (0);
	j = i;
	while (l[j] && !is_space(l[j]))
		j++;
	k = find_char(&l[i], c);
	if (k > 0 && k < j)
		return (0);
	return (1);
}

void	handle_awk_marks(char **tmp, int *i, char *line, char c)
{
	int	j;

	j = *i;
	(*tmp)[ft_strlen(*tmp)] = line[j - 1];
	while (line[j] && line[j] != c)
	{
		if (c == '\'' && line[j] == '$')
			(*tmp)[ft_strlen(*tmp)] = 127;
		else
			(*tmp)[ft_strlen(*tmp)] = line[j];
		j++;
	}
	(*tmp)[ft_strlen(*tmp)] = line[j];
	(*tmp)[ft_strlen(*tmp)] = line[j + 1];
	*i = j + 1;
}

void	handle_limiter_or_filename(char **tmp, int *i, char *line, char c)
{
	int	j;
	int	len;

	j = *i;
	j--;
	len = ft_strlen(*tmp);
	while (line[j] && line[j] != c)
	{
		(*tmp)[len++] = line[j];
		j++;
	}
	*i = j;
}
