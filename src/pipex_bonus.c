/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 11:51:13 by sfabi             #+#    #+#             */
/*   Updated: 2024/04/22 11:28:04 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	child(char *argv, t_env *env)
{
	int	pid;
	int	arr[2];

	if (pipe(arr) < 0)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		close(arr[0]);
		dup2(arr[1], 1);
		if (ft_execve(argv, env))
			return (-1);
	}
	else if (pid > 0)
	{
		close(arr[1]);
		dup2(arr[0], 0);
		waitpid(pid, NULL, 0);
	}
	return ((pid < 0 && !(close(arr[0]) + close(arr[1]))));
}

static	int	ft_heredoc(char **argv)
{
	char	*line;
	int		arr[2];
	int		pid;

	if (pipe(arr) < 0)
		return (-1);
	pid = fork();
	if (pid == 0 && !close(arr[0]))
	{
		while (write(1, "pipe heredoc> ", 14) && read_gnl(&line))
		{
			if (!ft_strncmp(line, argv[0], ft_strlen(argv[0]))
				&& line[ft_strlen(argv[0])] == '\n')
				return (close(arr[1]) + ft_free(line));
			write(arr[1], line, ft_strlen(line));
			free(line);
		}
	}
	else if (pid > 0)
	{
		dup2(arr[0], close(arr[1]));
		wait(NULL);
	}
	return ((pid < 0 && !(close(arr[0]) + close(arr[1]))));
}

int	pipex(t_data **data, char **argv, int fd_in, int fd_out)
{
	int	i;
	int	argc;
	int	ret;

	i = 0;
	argc = 0;
	while (argv[argc])
		argc++;
	if (fd_in == -10)
	{
		ret = ft_heredoc(argv);
		if (ret < 0)
			return (-1);
	}
	else
		(void)(dup2(fd_in, 0) + close(fd_in));
	while (++i < argc - 1)
	{
		ret = child(argv[i], (*data)->env);
		if (ret < 0)
			return (-1);
	}
	(void)(dup2(fd_out, 1) + close(fd_out));
	ret = ft_execve(argv[argc - 1], (*data)->env);
	return (ret);
}
