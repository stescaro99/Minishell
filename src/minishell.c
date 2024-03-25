/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 08:57:33 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/25 11:33:06 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	short	type_of_command(char *line)
{
	if ((!ft_strncmp(line, "exit", 4) && is_space(line[4]))
		|| (!ft_strncmp(line, "\"exit\"", 6) && is_space(line[6])))
		return (-1);
	else if (is_pipe(line))
		return (1);
	else if ((!ft_strncmp(line, "echo", 4) && is_space(line[4]))
		|| (!ft_strncmp(line, "\"echo\"", 6) && is_space(line[6])))
		return (2);
	else if ((!ft_strncmp(line, "cd", 2) && is_space(line[2]))
		|| (!ft_strncmp(line, "\"cd\"", 4) && is_space(line[4])))
		return (3);
	else if ((!ft_strncmp(line, "pwd", 3) && is_space(line[3]))
		|| (!ft_strncmp(line, "\"pwd\"", 5) && is_space(line[5])))
		return (4);
	else if ((!ft_strncmp(line, "export", 6) && is_space(line[6]))
		|| (!ft_strncmp(line, "\"export\"", 8) && is_space(line[8])))
		return (5);
	else if ((!ft_strncmp(line, "unset", 5) && is_space(line[5]))
		|| (!ft_strncmp(line, "\"unset\"", 7) && is_space(line[7])))
		return (6);
	else if ((!ft_strncmp(line, "env", 3) && is_space(line[3]))
		|| (!ft_strncmp(line, "\"env\"", 5) && is_space(line[5])))
		return (7);
	return (0);
}

short	ft_minishell(char *line, char **env)
{
	short	type;
	int		i;

	add_history(line);
	i = 0;
	while (is_space(line[i]))
		i++;
	type = type_of_command(&line[i]);
	if (type < 0)
		return (type);
	else if (type == 0)
		return (ft_execve(&line[i], env));
	else if (type == 7)
		return (print_env(env));
	else if (type == 2)
		handle_echo(&line[i], 1, 0);
	return (0);
}
