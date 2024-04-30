/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:23:08 by sfabi             #+#    #+#             */
/*   Updated: 2024/04/29 14:57:30 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_gnl(char **line)
{
	char	*buffer;
	int		i;
	int		r;
	char	c;

	i = 0;
	r = 0;
	buffer = (char *)malloc(10000);
	if (!buffer)
		return (-1);
	r = read(0, &c, 1);
	while (r && c != '\n' && c != '\0')
	{
		if (c != '\n' && c != '\0')
			buffer[i] = c;
		i++;
		r = read(0, &c, 1);
	}
	buffer[i] = '\n';
	buffer[++i] = '\0';
	*line = ft_strdup(buffer);
	if (!(*line))
		return (free(buffer), -1);
	free(buffer);
	return (r);
}

int	ft_free(char *s)
{
	free(s);
	return (1);
}

char	**add_str_front_to_matrix(char **mtx, char *s)
{
	char	**new_mtx;
	int		i;

	i = 0;
	while (mtx[i])
		i++;
	new_mtx = malloc(sizeof(char *) * (i + 2));
	if (!new_mtx)
		return (NULL);
	new_mtx[0] = ft_strdup(s);
	if (!new_mtx[0])
		return (free(new_mtx), ft_freesplit(mtx));
	i = -1;
	while (mtx[++i])
	{
		new_mtx[i + 1] = ft_strdup(mtx[i]);
		if (!new_mtx[i + 1])
		{
			ft_freesplit(mtx);
			return (ft_freesplit(new_mtx));
		}
	}
	ft_freesplit(mtx);
	new_mtx[i + 1] = NULL;
	return (free(s), new_mtx);
}

void	file_error(char *file)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd("No such file or directory", 2);
	ft_putstr_fd("\n", 2);
}

int	pipex_without_outfile(t_data *data, t_line **l, int fd_in, int i)
{
	char	**cmds;
	int		count_cat;
	int		res;
	char	*tmp;
	char	*line;

	line = (*l)->line;
	cmds = get_cmds(line, i, ft_strlen(line) - 1, fd_in);
	count_cat = 0;
	while (cmds[count_cat] && !strncmp(cmds[count_cat], "cat", 3)
		&& is_space(cmds[count_cat][3]))
		count_cat++;
	res = pipex(data, &cmds[count_cat - (!cmds[count_cat])], fd_in, 1);
	if (res < 0)
		(*l)->exit = -1;
	if (!count_cat || !cmds[count_cat])
		return (ft_freesplit(cmds), handle_logic(l), res);
	while (count_cat--)
	{
		read_gnl(&tmp);
		free(tmp);
		read_gnl(&tmp);
		free(tmp);
	}
	return (ft_freesplit(cmds), handle_logic(l), res);
}
