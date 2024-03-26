/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 08:32:55 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/26 11:06:01 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

short	handle_pwd(char	*line)
{
	char	*cwd;
	int		i;

	i = 4;
	while (line[i] && is_space(line[i]))
		i++;
	if (line[i])
		return (write(2, "pwd: too many arguments\n", 24) * 0);
	cwd = malloc(sizeof(char) * 1000);
	if (!cwd)
		return (0);
	if (getcwd(cwd, 1000))
		printf("%s\n", cwd);
	else
		return (free(cwd), 0);
	return (free(cwd), 0);
}

short	handle_cd(char *line)
{
	char	*cmd;
	int		i;

	i = 3;
	while (line[i] && is_space(line[i]))
		i++;
	if (!line[i] && !chdir(getenv("HOME")))
		return (0);
	cmd = malloc(sizeof(char) * 1000);
	if (!cmd)
		return (0);
	ft_strlcpy(cmd, &line[i], 1000);
	if (chdir(cmd))
	{
		write(2, "cd: no such file or directory: ", 32);
		write(2, cmd, ft_strlen(cmd));
		write(2, "\n", 1);
		return (free(cmd), 0);
	}
	return (free(cmd), 0);
}
