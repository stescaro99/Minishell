/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 09:55:24 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/18 11:18:09 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

short	is_limiter_or_filename(int i, char *l, char c)
{
	int	j;
	int	k;

	j = i;
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
