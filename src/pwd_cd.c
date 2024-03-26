/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 08:32:55 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/26 08:33:30 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

short	handle_pwd(void)
{
	char	*cwd;

	cwd = malloc(sizeof(char) * 1000);
	if (!cwd)
		return (write(2, "Error!\nduring malloc", 20) - 19);
	if (getcwd(cwd, 1000))
		printf("%s\n", cwd);
	else
		return (free (cwd), write(2, "Error!\nduring pwd", 17) - 16);
	return (free(cwd), 0);
}

short	handle_cd(char *line)
{
	char	*cmd;
	int		i;

	i = 2;
	if (!ft_strncmp(line, "cd", 3))
	{
		if (chdir(getenv("HOME")))
			return (1);
		return (0);
	}
	cmd = malloc(sizeof(char) * 1000);
	if (!cmd)
		return (write(2, "Error!\nduring malloc", 20) - 19);
	ft_strlcpy(cmd, line + 3, 1000);
	if (chdir(cmd))
	{
		write(2, "bash: cd: ", 10);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": No such file or directory\n", 28);
		return (free(cmd), 0);
	}
	return (free(cmd), 0);
}
