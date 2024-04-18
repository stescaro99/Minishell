/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 08:57:21 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/18 11:09:01 by stescaro         ###   ########.fr       */
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

static	char	*get_path(char *str, t_env *env)
{
	t_env	*node;
	char	**paths;
	char	*temp;
	char	*tmp;
	int		i;

	node = my_getenv(env, "PATH");
	if (!node)
		return (NULL);
	paths = ft_split(node->value, ':');
	free(node);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		tmp = ft_strjoin(temp, str);
		free(temp);
		if (access(tmp, F_OK) == 0)
			return (ft_freesplit(paths), tmp);
		i++;
		free(tmp);
	}
	return (ft_freesplit(paths), NULL);
}

static	char	*set_awk_arr(char *s, int *j, int i, int flag)
{
	if (i == 0)
		return (ft_strdup("awk"));
	if (flag == i)
		return (ft_strdup(&s[*j]));
	*j += find_char(&s[*j], '-') + 1;
	if (s[*j] == 'v')
		return (ft_strdup("-v"));
	else if (s[*j] == 'f')
		return (ft_strdup("-f"));
	else if (s[*j] == 'F')
		return (ft_strdup("-F"));
	else if (s[*j] == 'h')
		return (ft_strdup("-h"));
	else
		return (ft_strdup("--help"));
}

static	char	**clr_awk(char *str)
{
	int		i;
	int		flag;
	char	**clr;
	int		j;

	i = 0;
	flag = 0;
	while (i >= 0 && str[i])
	{
		i += find_char(&str[i], '-') + 1;
		if (i >= 0 && str[i] && (str[i] == 'v' || str[i] == 'f' || str[i] == 'F'
				|| str[i] == 'h' || (!ft_strncmp(&str[i], "-help", 5)
					&& is_space(str[i + 5]) && ++i)))
			flag++;
		else
			break ;
	}
	clr = ft_calloc(sizeof(char *), flag + 2);
	if (!clr)
		return (NULL);
	i = -1;
	j = 0;
	while (++i <= flag + 1)
		clr[i] = set_awk_arr(str, &j, i, flag + 1);
	return (clr);
}

short	ft_execve(char *str, t_env *env)
{
	int		pid;
	char	**clr;
	char	*path;
	int		res;

	res = 0;
	if (!ft_strncmp(str, "awk", 3) && is_space(str[3]))
		clr = clr_awk(&str[4]);
	else
		clr = ft_split(str, ' ');
	if (!clr)
		return (-1);
	control_inv(clr);
	path = get_path(clr[0], env);
	if (!path)
		return (ft_freesplit(clr), -1);
	pid = fork();
	if (pid == 0 && execve(path, clr, env->envp) < 0 && --res)
		return (ft_freesplit(clr), free(path), res);
	else if (pid > 0)
		waitpid(pid, &res, 0);
	free(path);
	if (!ft_strncmp(str, "clear", 5) && (!str[5] || is_space(str[5])))
		header();
	return (ft_freesplit(clr), res - (pid < 0));
}
