/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfabi <sfabi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 11:51:13 by sfabi             #+#    #+#             */
/*   Updated: 2024/04/26 15:47:55 by sfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	child(char *argv, t_env *env, int ret)
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
		waitpid(pid, &ret, 0);
	}
	return (WEXITSTATUS(ret) - (!(close(arr[0]) + close(arr[1])) && pid < 1));
}

static	int	children_exe(int argc, char **argv, t_data *data, int ret)
{
	int	i;

	i = -1;
	while (++i < argc)
	{
		ret += child(argv[i], data->env, 0);
		if (ret < 0)
			return (-1);
	}
	return (ret);
}

static void	ft_heredoc(char **argv)
{
	char	*line;
	int		arr[2];
	int		pid;

	if (pipe(arr) < 0)
		return ;
	pid = fork();
	if (pid < 0)
		return ;
	if (pid == 0 && !close(arr[0]))
	{
		while (write(1, "pipe heredoc> ", 14) && read_gnl(&line))
		{
			if (ft_strncmp(line, argv[0], ft_strlen(argv[0])) == 0
				&& line[ft_strlen(argv[0])] == '\n')
				exit(close(arr[1]) + ft_free(line));
			write(arr[1], line, ft_strlen(line));
			free(line);
		}
	}
	else
	{
		dup2(arr[0], close(arr[1]));
		wait(NULL);
	}
}

int	pipex(t_data *data, char **argv, int fd_in, int fd_out)
{
	int	argc;
	int	ret;
	int	or_stdout;
	int	or_stdin;

	ret = 0;
	argc = 0;
	or_stdin = dup(0);
	or_stdout = dup(1);
	while (argv[argc + 1])
		argc++;
	if (fd_in == -10)
		ft_heredoc(argv);
	else
		(void)(dup2(fd_in, 0) + close(fd_in));
	ret = children_exe(argc - (fd_in == -10), &argv[(fd_in == -10)], data, ret);
	(void)(dup2(fd_out, 1) + close(fd_out));
	ret += ft_execve(argv[argc], data->env);
	dup2(or_stdout, 1);
	dup2(or_stdin, 0);
	return (close(or_stdin) + close(or_stdout) + ret);
}
