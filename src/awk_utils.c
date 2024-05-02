/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   awk_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 13:31:25 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/26 15:31:08 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	n_flags_awk(char *str)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (i >= 0 && str[i])
	{
		i += find_char(&str[i], '-') + 1;
		if (i >= 0 && str[i] && (str[i] == 'v' || str[i] == 'f' || str[i] == 'F'
				|| str[i] == 'h' || (!ft_strncmp(&str[i], "-help", 5))))
			flag++;
		else
			break ;
	}
	return (flag);
}

int	is_file(char *str)
{
	int	i;

	i = find_char(str, '\'') + 1;
	i += find_char(&str[i], '\'') + 1;
	if (i < 0 || !str[i])
		return (0);
	while (str[i] && is_space(str[i]))
		i++;
	return (i);
}

char	*get_file(char *str, int i)
{
	if (str[i] == '\'')
		return (ft_substr(&str[i + 1], 0, find_char(&str[i + 1], '\'')));
	else if (str[i] == '\"')
		return (ft_substr(&str[i + 1], 0, find_char(&str[i + 1], '\"')));
	else
		return (ft_strdup(&str[i]));
}

char	*set_flag(char *s, int *j)
{
	int		i;
	char	*flag;

	i = find_char(&s[*j], '-') + 1;
	flag = malloc(3);
	if (!flag)
		return (NULL);
	flag[0] = '-';
	flag[1] = s[*j + i];
	flag[2] = 0;
	*j += i + 1;
	return (flag);
}
