/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 09:30:06 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/29 14:45:38 by stescaro         ###   ########.fr       */
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

static	int	handle_ref(t_memory *memory, t_env *env, char *tmp, char **ret)
{
	int			j;
	char		*ref;

	j = len_ref(tmp);
	ref = ft_substr(tmp, 0, j);
	if (!ref)
		return (j);
	while (memory)
	{
		if (!ft_strncmp(memory->name, ref, ft_strlen(ref)))
			ft_strlcat(*ret, memory->value, 25000);
		if (!ft_strncmp(memory->name, ref, ft_strlen(ref)))
			return (free(ref), j);
		memory = memory->next;
	}
	while (env)
	{
		if (!ft_strncmp(env->name, ref, ft_strlen(ref)))
		{
			ft_strlcat(*ret, env->value, 25000);
			return (free(ref), j);
		}
		env = env->next;
	}
	return (free(ref), j);
}

char	*check_ref(char *tmp, t_memory *memory, t_env *env)
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
		else if (tmp[i] == 127 || is_space(tmp[i + 1])
			|| tmp[i + 1] == 127 || tmp[i + 1] == '$')
			ret[j] = 36;
		else if (i > 0 && tmp[i] == '$' && tmp[i + 1] == '?' && ++i)
			ret[j] = 127;
		else if (++i)
			i += handle_ref(memory, env, &tmp[i], &ret);
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

short	nq(char *s, int times)
{
	int		i;
	char	*left;

	i = 0;
	while (times)
	{
		while (s[i] && strncmp(&s[i], "/bin/", 5))
			i++;
		if (s[i])
		{
			i += 5;
			times--;
		}
	}
	left = ft_substr(s, 0, i - 5);
	if ((odd_quotes(left, '\'') && odd_quotes(&s[i], '\''))
		|| (odd_quotes(left, '\"') && odd_quotes(&s[i], '\"')))
		return (free(left), 0);
	return (free(left), 1);
}
