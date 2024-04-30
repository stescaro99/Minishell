/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfabi <sfabi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 13:40:16 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/29 13:40:49 by sfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	handle_par_logic(t_line **l, t_line **par)
{
	int		e;

	e = 0;
	if ((*par)->exit == 10)
	{
		(*l)->exit = 10;
		return ;
	}
	if ((*par)->exit == 0 && ++e)
		(*par)->exit = 1;
	else if (--e)
		(*par)->exit = -1;
	handle_logic(par);
	if (!(*par))
	{
		(*l)->exit = e;
		handle_logic(l);
	}
}

static	void	handle_miniparentesis(t_data *data, t_line **line, t_line **par)
{
	t_line	*temp;
	int		res;
	int		pid;

	if (!par || !*par)
		return ;
	temp = *par;
	res = -1;
	pid = fork();
	if (pid == 0)
	{
		res = minishell(data, temp, 0, 10);
		exit(res);
	}
	else if (pid > 0)
		waitpid(pid, &res, 0);
	(*par)->exit = WEXITSTATUS(res);
	handle_par_logic(line, par);
}

static	int	shell_cmds(t_data **data, t_line **tmp, t_env *path_node)
{
	int	unset;

	unset = path_node->unsetted;
	if ((*tmp)->type == 1)
		return (pipe_shell(*data, tmp));
	else if ((*tmp)->type == 2 && !unset)
		return (echo_shell(tmp));
	else if ((*tmp)->type == 2 && (*tmp)->exit--)
		return (file_error("echo"), handle_logic(tmp), 0);
	else if ((*tmp)->type == 3)
		return (cd_shell(*data, tmp));
	else if ((*tmp)->type == 4)
		return (pwd_shell(tmp));
	else if ((*tmp)->type == 5)
		return (export_shell(&((*data)->env), tmp));
	else if ((*tmp)->type == 6)
		return (unset_shell(&((*data)->env), tmp));
	else if ((*tmp)->type == 7 && !unset)
		return (env_shell((*data)->env, tmp));
	else if ((*tmp)->type == 7 && (*tmp)->exit--)
		return (file_error("env"), handle_logic(tmp), 0);
	else if ((*tmp)->type == 8)
		return (memory_shell(*data, tmp));
	else
		return (exec_shell(*data, tmp));
}

int	minishell(t_data *data, t_line *tmp, int ret, int ext)
{
	t_env	*path_node;

	path_node = my_getenv(data->env, "PATH");
	while (tmp)
	{
		if (tmp->type == -2)
			return (ret);
		else if (tmp->type == -1 || tmp->exit > 9)
			return (ext);
		else if (tmp->type < 9 && tmp->type > -1)
			ret += shell_cmds(&data, &tmp, path_node);
		else if (tmp->type == 9)
		{
			handle_miniparentesis(data, &tmp, &(tmp->parentesis));
			tmp = tmp->next;
		}
	}
	return (ret);
}
