/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 08:57:21 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/25 08:57:22 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	char	*get_path(char *str)
{
	char	*path;
	char	**paths;
	char	*temp;
	char	*tmp;
	int		i;

	path = getenv("PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		tmp = ft_strjoin(temp, str);
		free(temp);
		if (access(tmp, F_OK) == 0)
			return (ft_freesplit(paths), tmp);
		i++;
		free(tmp);
	}
	return (ft_freesplit(paths), NULL);
}

short	ft_execve(char *str, char **env)
{
	int		pid;
	char	**clr;
	char	*path;

	clr = ft_split(str, ' ');
	if (!clr)
		return (perror("Allocation failed"), 1);
	path = get_path(clr[0]);
	if (!path)
		return (ft_freesplit(clr), 1);
	pid = fork();
	if (pid < 0)
		return (perror("Fork failed"), 1);
	if (pid == 0)
	{
		if (execve(path, clr, env) < 0)
			return (ft_freesplit(clr), free(path), perror("Execve failed"), 1);
	}
	else
		waitpid(pid, NULL, 0);
	ft_freesplit(clr);
	if (path)
		free(path);
	return (0);
}
