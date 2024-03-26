/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:06:02 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/25 14:29:49 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_matdup(char **env)
{
	int		i;
	char	**new;

	i = 0;
	while (env[i])
		i++;
	new = (char **)malloc(sizeof(char *) * (i + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new[i] = ft_strdup(env[i]);
		i++;
	}
	new[i] = NULL;
	return (new);
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
