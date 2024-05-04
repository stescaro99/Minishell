/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfabi <sfabi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 08:57:21 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/03 16:00:34 by sfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static	int	set_sig(char flag)
{
	if (flag == 1)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (flag == 2)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else
	{
		signal(SIGINT, signal_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	return (1);
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

short	ft_execve(char *str, t_env *env, int res)
{
	int		pid;
	char	**clr;
	char	*path;

	if (!strncmp(str, "bash", 4) && str[4] == '@')
		clr = ft_split(str, '@');
	else if ((!strncmp(str, "awk", 3) && is_space(str[3]))
		|| ((!strncmp(str, "\"awk\"", 5) || !strncmp(str, "\'awk\'", 5))
			&& is_space(str[5])))
		clr = clr_awk(&str[4 + 2 * (str[0] == '\'' || str[0] == '\"')]);
	else
		clr = ft_split(str, ' ');
	path = get_path(clr[0], env);
	if (!path || !path[0])
		return (ft_freesplit(clr), set_sig(0) * 0 -1 + (path && ft_free(path)));
	pid = fork();
	if (pid == 0 && set_sig(2) && execve(path, clr, env->envp) < 0 && --res)
		return (ft_freesplit(clr), free(path), set_sig(0) * 0 + res);
	else if (pid > 0 && set_sig(1))
		waitpid(pid, &res, 0);
	if (ft_free(path) && !ft_strncmp(str, "clear", 5) && is_space(str[5]))
		header();
	return (ft_freesplit(clr), set_sig(0), WEXITSTATUS(res) - (pid < 0));
}
