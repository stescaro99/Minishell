/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 09:14:14 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/03 14:44:51 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	is_command(char *str)
{
	char	**path;
	char	*tmp;
	char	*tmp2;
	char	*s;
	int		i;

	s = ft_substr(str, 0, len_to_first_space(str));
	path = ft_split(getenv("PATH"), ':');
	i = 0;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		tmp2 = ft_strjoin(tmp, s);
		free(tmp);
		if (access(tmp2, F_OK) == 0)
		{
			ft_freesplit(path);
			return (free(tmp2), free(s), 1);
		}
		free(tmp2);
		i++;
	}
	return (free(s), 0);
}

static	void	null_res(char **res)
{
	int	len;
	int	j;

	len = ft_strlen(*res);
	j = len;
	while (j > 0 && !is_space((*res)[j]))
		j--;
	while (j > 0 && is_space((*res)[j]))
		j--;
	while (j > 0 && !is_space((*res)[j]) && (*res)[j] != '|' && (*res)[j] != ';'
		&& (*res)[j] != '<' && (*res)[j] != '>' && (*res)[j] != '&'
		&& (*res)[j] != '(' && (*res)[j] != ')' && (*res)[j] != '\''
		&& (*res)[j] != '\"' && ((*res)[j] < 15 || (*res)[j] > 19)
		&& (*res)[j] != 127)
		j--;
	if (j > 0)
		j++;
	while (j < len)
	{
		(*res)[j] = 0;
		j++;
	}
}

static	int	set_start(char *s, int i)
{
	while (i > 0 && !is_space(s[i]))
		i--;
	while (i > 0 && is_space(s[i]))
		i--;
	while (i > 0 && !is_space(s[i]) && s[i] != '|' && s[i] != ';' && s[i] != '<'
		&& s[i] != '>' && s[i] != '&' && s[i] != '(' && s[i] != ')'
		&& s[i] != '\'' && s[i] != '\"')
		i--;
	return (i + (i > 0));
}

int	handle_wcards(char *line, int i, char **res)
{
	int		start;
	int		end;
	char	*tmp;

	start = set_start(line, i);
	end = i;
	while (line[end] && !is_space(line[end]) && line[end] != '|'
		&& line[end] != ';' && line[end] != '<' && line[end] != '>'
		&& line[end] != '&' && line[end] != '(' && line[end] != ')'
		&& line[end] != '\'' && line[end] != '\"')
		end++;
	tmp = remove_spaces(ft_substr(line, start, end - start));
	if (!is_command(tmp))
	{
		free(tmp);
		(*res)[ft_strlen(*res)] = line[i];
		return (i);
	}
	null_res(res);
	ft_strlcat((*res), "bash@-c@", 25000);
	ft_strlcat((*res), tmp, 25000);
	return (free(tmp), end - 1);
}
