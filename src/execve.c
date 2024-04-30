/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user-x <user-x@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 08:57:21 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/30 12:50:27 by user-x           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	control_inv(char **clr)
{
	char	*temp;

	if (clr[0][0] != '\"')
		return ;
	temp = ft_substr(clr[0], 1, find_char(&clr[0][1], '\"'));
	free(clr[0]);
	clr[0] = temp;
}

char	*get_path(char *str, t_env *env)
{
	t_env	*node;
	char	**paths;
	char	*temp;
	char	*tmp;
	int		i;

	node = my_getenv(env, "PATH");
	if (!node)
		return (NULL);
	if (node->unsetted == 1)
		return (file_error(str), ft_strdup(""));
	paths = ft_split(node->value, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		tmp = ft_strjoin(temp, str);
		free(temp);
		if (access(tmp, F_OK) == 0)
			return (ft_freesplit(paths), tmp);
		free(tmp);
	}
	return (ft_freesplit(paths), NULL);
}

static	char	**clr_awk(char *str)
{
	int		i;
	int		j;
	int		flag;
	int		file;
	char	**clr;

	flag = n_flags_awk(str);
	file = is_file(str);
	clr = ft_calloc(sizeof(char *), flag + (file > 0) + 3);
	if (!clr)
		return (NULL);
	clr[0] = ft_strdup("awk");
	i = -1;
	j = 0;
	while (++i < flag)
		clr[i + 1] = set_flag(str, &j);
	if (file > 0)
	{
		clr[i + 1] = ft_substr(str, j + 1, file - j - 4);
		clr[i + 2] = get_file(str, file);
	}
	else
		clr[i + 1] = ft_substr(str, j + 1, ft_strlen(str) - j - 2);
	return (clr);
}

short	ft_execve(char *str, t_env *env)
{
	int		pid;
	char	**clr;
	char	*path;
	int		res;

	res = 0;
	if ((!strncmp(str, "awk", 3) && is_space(str[3]))
		|| ((!strncmp(str, "\"awk\"", 5) || !strncmp(str, "\'awk\'", 5))
			&& is_space(str[5])))
		clr = clr_awk(&str[4 + 2 * (str[0] == '\'' || str[0] == '\"')]);
	else
		clr = ft_split(str, ' ');
	control_inv(clr);
	path = get_path(clr[0], env);
	if (!path || !path[0])
		return (ft_freesplit(clr), -1 + (path && ft_free(path)));
	pid = fork();
	if (++g_ctrl && pid == 0 && execve(path, clr, env->envp) < 0 && --res)
		return (ft_freesplit(clr), free(path), res);
	else if (pid > 0)
		waitpid(pid, &res, 0);
	free(path);
	if (!ft_strncmp(str, "clear", 5) && (!str[5] || is_space(str[5])))
		header();
	return (ft_freesplit(clr), --g_ctrl + WEXITSTATUS(res) - (pid < 0));
}