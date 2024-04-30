/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfabi <sfabi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 11:51:13 by sfabi             #+#    #+#             */
/*   Updated: 2024/04/29 13:18:21 by sfabi            ###   ########.fr       */
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
		exit(ft_execve(argv, env));
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
		if (ret < 0 || ret > 26)
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

static	void	check_cmds(int ret, int argc, char **argv, t_env *env)
{
	char	*cmd;
	char	*path;
	int		i;
	short	useless;

	if (ret > -1 && ret < 2)
		return ;
	i = 0;
	useless = 0;
	while (i < argc + 1)
	{
		cmd = ft_substr(argv[i], 0, find_char(argv[i], ' '));
		path = get_path(cmd, env);
		if (!path)
			cmd_not_found(cmd, &useless);
		else if (path[0] == '\0')
			file_error(cmd);
		if (path)
			free(path);
		free(cmd);
		i++;
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
	else if (fd_in)
	{
		dup2(fd_in, 0);
		close(fd_in);
	}
	ret = children_exe(argc - (fd_in == -10), &argv[(fd_in == -10)], data, ret);
	(void)(fd_out == 1 || (dup2(fd_out, 1) + close(fd_out)));
	ret += ft_execve(argv[argc], data->env);
	dup2(or_stdout, 1);
	dup2(or_stdin, 0);
	check_cmds(ret, argc - (fd_in == -10), &argv[(fd_in == -10)], data->env);
	return ((close(or_stdin) + close(or_stdout)) * 0);
}
