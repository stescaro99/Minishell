/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:06:02 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/26 12:08:17 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_err(short r, char *line)
{
	if (r < 1 || !line[0])
		return (free(line));
	else if (r == 1)
	{
		write(2, "zsh: command not found: ", 25);
		write(2, line, len_to_first_space(line));
		write(2, "\n", 1);
	}
	free(line);
}

char	*echo_readline(char *prompt)
{
	char	*line;
	char	*temp;

	temp = readline(prompt);
	line = ft_strjoin("\n", temp);
	free(temp);
	return (line);
}

int	len_to_first_space(char *s)
{
	int	i;

	i = 0;
	while (s[i] && !is_space(s[i]))
		i++;
	return (i);
}

int	find_char(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (0);
}
