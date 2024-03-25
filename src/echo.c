/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 10:31:43 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/25 11:51:10 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	odd_quotes(char *line)
{
	int		i;
	int		quotes;

	i = 0;
	quotes = 0;
	while (line[i])
	{
		if (line[i] == '"')
			quotes++;
		i++;
	}
	if (quotes % 2 == 0)
		return (0);
	return (1);
}


static	void	print_quotes(char *line)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (line[i])
	{
		if (line[i] == '"')
			i++;
		j = i;
		while (line[j] && line[j] != '"')
			j++;
		temp = ft_substr(line, i, j - i);
		printf("%s", temp);
		free(temp);
		i = j + 1 + (line[j + 1] == '"');
	}
}

static	void	handle_dquote(char *line, int quotes, short flag)
{
	char	*temp;
	char	*new_line;
	char	*l;

	l = ft_strdup(line);
	while (1)
	{
		temp = ft_readline("dquote> ");
		if (odd_quotes(temp))
		{
			new_line = ft_strjoin(l, temp);
			free(l);
			free(temp);
			l = new_line;
			handle_echo(l, 0, flag);
			return (free(new_line));
		}
		else
		{
			new_line = ft_strjoin(l, temp);
			free(l);
			l = new_line;
			free(temp);
		}
	}
}

static	void	print_line(char *line, short flag)
{
	int		quotes;
	int		i;

	i = 0;
	quotes = 0;
	while (line[i])
	{
		if (line[i] == '"')
			quotes++;
		i++;
	}
	if (quotes == 0)
		printf("%s", line);
	else if (quotes % 2 == 0)
		print_quotes(line);
	else
		handle_dquote(line, quotes, flag);
}

void	handle_echo(char *line, short q, short f)
{
	int		i;
	short	flag;

	flag = f;
	i = 0;
	if (line[0] == '\"' && q)
		i = 7;
	else if (q)
		i = 5;
	if (q && line[i] == '-' && line[i + 1] == 'n')
	{
		i += 3;
		flag = 1;
	}
	print_line(&line[i], flag);
	if (q && !flag)
		printf("\n");
	else if (q)
		printf("\e[38;2;0;0;0m\e[48;2;255;255;255m%%\033[0m\n");
}
