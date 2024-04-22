/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 09:27:31 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/22 12:00:59 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	heredoc(char *limiter, char *cmd, t_data *data)
{
	int		pipe_fd[2];
	int		res;
	char	*line;
	size_t	len;

	if (pipe(pipe_fd))
		return (-1);
	len = ft_strlen(limiter);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || (!ft_strncmp(line, limiter, len) && !line[len]))
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);
	res = ft_execve(cmd, data->env);
	return (res);
}

static	int	ft_redir_in(t_data *data, char *cmd, char *file_name, short type)
{
	int		fd;
	int		pipe_fd[2];
	int		res;

	if (type == 2)
		return (heredoc(file_name, cmd, data));
	fd = open(file_name, O_RDONLY, 0644);
	if (fd < 0)
		return (fd);
	if (pipe(pipe_fd))
		return (-1);
	close(pipe_fd[0]);
	dup2(fd, 0);
	close(fd);
	dup2(pipe_fd[1], 1);
	close(pipe_fd[1]);
	res = ft_execve(cmd, data->env);
	return (res);
}

static	int	ft_redir_out(t_data *data, char *cmd, char *file_name, short type)
{
	int		fd;
	int		res;
	int		pipe_fd[2];

	if (type == 2)
		fd = open(file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (fd);
	pipe(pipe_fd);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);
	dup2(fd, 1);
	close(fd);
	res = ft_execve(cmd, data->env);
	return (res);
}

int	ft_redir(t_data *data, t_line *line)
{
	char	*cmd;
	char	*file_name;
	int		i;
	short	type;

	i = find_char(line->line, '>');
	type = 1;
	if (i < 0)
		i = find_char(line->line, '<');
	if ((line->line[i] == '>' && line->line[i + 1] == '>')
		|| (line->line[i] == '<' && line->line[i + 1] == '<'))
		type++;
	if (i != 0)
		cmd = ft_substr(line->line, 0, i);
	if (i != 0)
		file_name = ft_substr(line->line, i + type, ULONG_MAX);
	else
	{
		file_name = ft_substr(line->line, i + type,
				len_to_first_space(&line->line[i + type]));
		cmd = ft_substr(line->line, i + type + ft_strlen(file_name), ULONG_MAX);
	}
	if (find_char(line->line, '>') > 0)
		return (ft_redir_out(data, cmd, file_name, type));
	return (ft_redir_in(data, cmd, file_name, type));
}

int	exec_shell(t_data *data, t_line **line)
{
	t_line	*temp;
	char	*l;
	short	res;

	temp = *line;
	l = temp->line;
	if (find_char(l, '>') < 0 && find_char(l, '<') < 0)
		res = ft_execve(l, data->env);
	else
		res = ft_redir(data, temp);
	if (res < 0)
	{
		//cmd_not_found(l, res);
		temp->exit = -1;
	}
	handle_logic(line);
	return ((int)res);
}
