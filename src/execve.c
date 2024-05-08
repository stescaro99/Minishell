/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 08:57:21 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/06 12:27:43 by stescaro         ###   ########.fr       */
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

static	void	sig_new(int sig)
{
	if (sig == SIGINT)
	{
		g_ctrl = 130;
		printf("\n");
		signal(SIGINT, SIG_IGN);
	}
	else if (sig == SIGQUIT)
	{
		g_ctrl = 131;
		printf("\n");
		signal(SIGQUIT, SIG_IGN);
	}
}

static	int	s_s(char flag, int *res)
{
	if (flag == 1)
	{
		signal(SIGINT, sig_new);
		signal(SIGQUIT, sig_new);
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
		if (g_ctrl == 130)
			*res = 130;
		else if (g_ctrl == 131)
			*res = 131;
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

	if (!str)
		return (NULL);
	flag = n_flags_awk(str);
	file = is_file(str);
	clr = ft_calloc(sizeof(char *), flag + (file > 0) + 3);
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
	else if ((!strncmp(str, "awk", 3) && is_space(str[3])))
		clr = clr_awk(&str[4 + 2 * (str[0] == '\'' || str[0] == '\"')
				- (!str[3 + 2 * (str[0] == '\'' || str[0] == '\"')])]);
	else
		clr = ft_split(str, ' ');
	if (!clr)
		return (s_s(0, &res) * 0);
	path = get_path(clr[0], env);
	if (!path || !path[0])
		return (ft_freesplit(clr), s_s(0, &res) * 0 - !(path && ft_free(path)));
	pid = fork();
	if (pid == 0 && s_s(2, NULL) && execve(path, clr, env->envp) < 0 && --res)
		return (ft_freesplit(clr), free(path), s_s(0, &res) * 0 + res);
	else if (pid > 0 && s_s(1, NULL))
		waitpid(pid, &res, 0);
	if (ft_free(path) && !ft_strncmp(str, "clear", 5) && is_space(str[5]))
		header();
	res = WEXITSTATUS(res);
	return (ft_freesplit(clr), s_s(0, &res) * 0 + res - (pid < 0));
}
