/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 08:25:35 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/03 14:10:00 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	char	*delete_c(char *line, char c, int flag)
{
	char	*ret;
	int		i;
	int		j;

	if (flag)
		return (ft_strdup(line));
	ret = ft_calloc(ft_strlen(line), 1);
	if (!ret)
		return (NULL);
	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] != c)
			ret[j++] = line[i];
		i++;
	}
	return (ret);
}

int	odd_quotes(char *line, char c)
{
	int		i;
	int		quotes;

	i = 0;
	quotes = 0;
	while (line[i])
	{
		if (line[i] == c)
			quotes++;
		i++;
	}
	if (quotes % 2 == 0)
		return (0);
	return (1);
}

static	char	*handle_dquote(char c, int f)
{
	char	*prompt;
	char	*ret;
	char	*tmp;
	short	flag;

	prompt = ft_calloc(9 - (c == '\''), 1);
	if (!prompt)
		return (NULL);
	if (c == '\'')
		ft_strlcpy(prompt, "quote> ", 8);
	else
		ft_strlcpy(prompt, "dquote> ", 9);
	flag = 1;
	ret = NULL;
	while (flag)
	{
		tmp = readline(prompt);
		if (odd_quotes(tmp, c))
			flag = 0;
		tmp = join_and_free_sec("\n", tmp);
		ret = f_strjoin(ret, tmp);
		free(tmp);
	}
	tmp = delete_c(ret, c, f);
	return (free(prompt), free(ret), tmp);
}

static	void	handle_odd_quotes(char **tmp, int flag, int j, char c)
{
	char	*dq;
	int		k;

	dq = handle_dquote(c, flag);
	k = -1;
	while (dq[++k])
	{
		if (dq[k] != '$' && dq[k] != '(' && dq[k] != ')')
			(*tmp)[j++] = dq[k];
		else if (dq[k] == '$')
			(*tmp)[j++] = 36 + ((c == '\'') * 91);
		else
			(*tmp)[j++] = (dq[k] == ')') + 18;
	}
	free(dq);
}

void	handle_quote(char **tmp, int *i, char *l, char c)
{
	int		flag;

	flag = is_limiter_or_filename(*i, l, c);
	if (!flag)
	{
		while (l[*i] && l[*i] != c)
		{
			if (l[(*i)] != '$' && l[(*i)] != '(' && l[(*i)] != ')')
				(*tmp)[ft_strlen(*tmp)] = l[(*i)];
			else if (l[(*i)] == '$')
				(*tmp)[ft_strlen(*tmp)] = 36 + ((c == '\'') * 91);
			else if (l[(*i)] == '(' || l[(*i)] == ')')
				(*tmp)[ft_strlen(*tmp)] = (l[(*i)] == ')') + 18 - 2 * (c == 39);
			else if (l[(*i)] == '*')
				(*tmp)[ft_strlen(*tmp)] = 15;
			(*i)++;
		}
	}
	else if (flag > 0)
		handle_limiter_or_filename(tmp, i, l, c);
	else
		handle_awk_marks(tmp, i, l, c);
	if (!(l[*i]) && flag >= 0)
		handle_odd_quotes(tmp, flag, ft_strlen(*tmp), c);
	(*i) += 1 - (flag < 0);
}
