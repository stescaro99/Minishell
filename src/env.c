/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 08:57:13 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/25 14:33:43 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	char	**set_new_env(char **env, int j)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * i);
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < j)
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	while (env[j + 1])
	{
		new_env[j] = ft_strdup(env[j + 1]);
		j++;
	}
	return (new_env);
}

static	int	size_of_cmd(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '=')
		i++;
	return (i);
}

short	handle_unset(char *line, char **env)
{
	int		i;
	int		j;
	char	**new_env;

	i = 6;
	if (line[0] == '"')
		i = 8;
	while (is_space(line[i]))
		i++;
	j = 0;
	while (env[j] && ft_strncmp(env[j], &line[i], size_of_cmd(env[j])))
		j++;
	if (!env[j])
		return (6);
	new_env = set_new_env(env, j);
	if (!new_env)
		return (6);
	ft_freesplit(env);
	env = ft_matdup(new_env);
	ft_freesplit(new_env);
	return (0);
}

short	print_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (0);
}
