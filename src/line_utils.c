/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 09:30:06 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/24 08:48:01 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	len_ref(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '\'' && !is_space(s[i])
		&& s[i] != '$' && s[i] != '\"')
		i++;
	return (i);
}

static	void	handle_ref(t_memory *memory, int *i, char *tmp, char **ret)
{
	int			j;
	char		*ref;

	if (is_space(tmp[0]))
	{
		(*ret)[ft_strlen(*ret)] = 36;
		return ;
	}
	j = len_ref(tmp);
	*i += j;
	ref = ft_substr(tmp, 0, j);
	if (!ref)
		return ;
	while (memory)
	{
		if (!ft_strncmp(memory->name, ref, ft_strlen(ref)))
		{
			ft_strlcat(*ret, memory->value, 25000);
			return (free(ref));
		}
		memory = memory->next;
	}
	free(ref);
}

char	*check_ref(char *tmp, t_memory *memory)
{
	int		i;
	char	*ret;
	int		j;

	i = -1;
	ret = ft_calloc(25000, 1);
	if (!ret)
		return (NULL);
	while (tmp[++i])
	{
		j = ft_strlen(ret);
		if (tmp[i] != 127 && tmp[i] != '$')
			ret[j] = tmp[i];
		else if (tmp[i] == 127)
			ret[j] = 36;
		else if (++i)
			handle_ref(memory, &i, &tmp[i], &ret);
	}
	return (ret);
}

void	line_lstclear(t_line *lst)
{
	t_line	*temp;

	while (lst)
	{
		temp = lst->next;
		if (lst->parentesis)
			line_lstclear(lst->parentesis);
		free(lst->line);
		free(lst);
		lst = temp;
	}
}
