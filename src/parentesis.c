/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentesis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 12:06:33 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/05 12:20:29 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_line	*set_parentesis(char *line)
{
	char	*tmp;

	if (line[0] != '(')
		return (NULL);
	tmp = ft_substr(line, 1, ft_strlen(line) - 2);
	return (ft_line_lstnew(tmp));
}
