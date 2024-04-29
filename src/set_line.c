/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:28:31 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/29 14:25:56 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_marks(char *line, t_memory *memory, t_env *env)
{
	char			*tmp;
	int				i;
	char			*ret;
	unsigned int	flag;

	tmp = ft_calloc(25000, 1);
	if (!tmp)
		return (NULL);
	i = 0;
	while (line[i] && is_space(line[i]))
		i++;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '\"') && ++i)
			handle_quote(&tmp, &i, line, line[i - 1]);
		else
			tmp[ft_strlen(tmp)] = line[i++];
		flag = 0;
		while (line[i] && is_space(line[i]) && ++flag)
			i++;
		if (flag && line[i])
			tmp[ft_strlen(tmp)] = ' ';
	}
	ret = check_ref(tmp, memory, env);
	return (free(tmp), ret);
}

static	int	handle_parenthesis(char *line, int j)
{
	int		flag;

	flag = 0;
	while (line[j])
	{
		if (line[j] == '(')
			flag++;
		else if (line[j] == ')')
			flag--;
		if (flag == 0 && line[j] == ')')
			return (j);
		j++;
	}
	return (j);
}

static	int	set_end(char *line)
{
	int	i;
	int	j;

	i = 0;
	while (line[i] && is_space(line[i]))
		i++;
	while (line[i] && line[i] != '&' && (line[i] != '|' || !line[i + 1]
			|| line[i + 1] != '|') && line[i] != ';')
		i++;
	j = find_char(line, '(');
	if (j == 0)
		i = handle_parenthesis(line, j) + 1;
	else if (j != -1 && j < i)
		i = j;
	return (i);
}

t_line	*ft_line_lstnew(char *line)
{
	t_line	*new;
	int		end;
	int		start;

	if (!line)
		return (NULL);
	if (line[0] == '\0')
		return (new_empty_line());
	new = (t_line *)malloc(sizeof(t_line));
	if (!new)
		return (NULL);
	start = skip_spaces(line);
	end = set_end(&line[start]);
	new->line = ft_substr(line, start, end);
	if (!new->line)
		return (free(new), NULL);
	new->parentesis = set_parentesis(new->line);
	new->type = find_type(new->line);
	new->logic = set_logic(&line[start + end]);
	new->exit = 1;
	while (line[start + end] && is_space(line[start + end]))
		end++;
	new->next = ft_line_lstnew(&line[start + end + 2 * (new->logic > 0)
			+ (new->logic == -1)]);
	return (new);
}

t_line	*line_lstnew(char *line, t_memory *memory, t_env *env)
{
	char	*tmp;
	t_line	*new;

	tmp = remove_marks(line, memory, env);
	if (!tmp)
		return (NULL);
	new = ft_line_lstnew(tmp);
	free(tmp);
	return (new);
}
