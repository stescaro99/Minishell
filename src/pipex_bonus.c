/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 11:51:13 by sfabi             #+#    #+#             */
/*   Updated: 2024/04/26 10:40:23 by stescaro         ###   ########.fr       */
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

static	int	children_exe(int argc, char **argv, t_data *data, int ret)
{
	int	i;

	i = -1;
	while (++i < argc)
	{
		ret += child(argv[i], data->env);
		if (ret < 0)
			return (-1);
	}
	return (ret);
}

static	int	heredoc_pipex(t_data *data, char **argv, int fd_out)
{
	int	argc;
	int	ret;
	int	or_stdout;

	ret = 0;
	argc = 0;
	while (argv[argc + 1])
		argc++;
	ret = children_exe(argc, argv, data, ret);
	or_stdout = dup(1);
	(void)(dup2(fd_out, 1) + close(fd_out));
	ret += ft_execve(argv[argc], data->env);
	dup2(or_stdout, 1);
	return (close(or_stdout) + ret);
}

static	int	ft_heredoc(t_data *data, char **argv, int fd_out)
{
	int	arr[2];
	int	pid;
	int	res;
	int	or_stdin;

	res = 0;
	if (pipe(arr) < 0)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		close(arr[0]);
		get_heredoc_infile(arr[1], argv[0]);
		res = heredoc_pipex(data, &argv[1], fd_out);
		return (res);
	}
	else if (pid > 0)
	{
		or_stdin = dup(0);
		dup2(arr[0], close(arr[1]));
		waitpid(pid, &res, 0);
		dup2(or_stdin, 0);
		close(or_stdin);
	}
	return (WEXITSTATUS(res) - (!(close(arr[0]) + close(arr[1])) && pid < 0));
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
	while (argv[argc + 1])
		argc++;
	if (fd_in == -10)
	{
		ret = ft_heredoc(data, argv, fd_out);
		return ((dup2(or_stdin, 0) + close(or_stdin)) * 0 + ret);
	}
	else
		(void)(dup2(fd_in, 0) + close(fd_in));
	ret = children_exe(argc - (fd_in == -10), &argv[(fd_in == -10)], data, ret);
	or_stdout = dup(1);
	(void)(dup2(fd_out, 1) + close(fd_out));
	ret += ft_execve(argv[argc], data->env);
	dup2(or_stdout, 1);
	dup2(or_stdin, 0);
	return (close(or_stdin) + close(or_stdout) + ret);
}

/*sistemare heredoc*/