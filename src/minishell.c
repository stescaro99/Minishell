/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 13:40:16 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/22 11:35:00 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	handle_par_logic(t_line **l, t_line *par)
{
	t_line	*line;

	line = *l;
	if (par->exit == -10)
	{
		while (line)
			line = line->next;
	}
	else if (par->exit != 0)
	{
		line->exit = par->exit;
		handle_logic(l);
	}
	else if (par->exit == 0)
	{
		handle_logic(l);
	}
}

static	void	handle_miniparentesis(t_data *data, t_line **line, t_line **par)
{
	t_line	*temp;
	int		res;
	int		pid;

	if (!par || !(*par))
		return ;
	temp = *par;
	res = -1;
	pid = fork();
	if (pid == 0)
	{
		res = minishell(data, temp, 0, -10);
		exit(res);
	}
	else if (pid > 0)
		waitpid(pid, &res, 0);
	(*par)->exit = res;
	handle_par_logic(line, temp);
}

int	minishell(t_data *data, t_line *tmp, int ret, int ext)
{
	while (tmp)
	{
		handle_miniparentesis(data, &tmp, &(tmp->parentesis));
		if (tmp->type == -1 || tmp->exit > 9)
			return (ext);
		else if (tmp->type == 1)
			ret += pipe_shell(data, &tmp);
		else if (tmp->type == 2)
			ret += echo_shell(&tmp);
		else if (tmp->type == 3)
			ret += cd_shell(data, &tmp);
		else if (tmp->type == 4)
			ret += pwd_shell(&tmp);
		else if (tmp->type == 5)
			ret += export_shell(&(data->env), &tmp);
		else if (tmp->type == 6)
			ret += unset_shell(&(data->env), &tmp);
		else if (tmp->type == 7)
			ret += env_shell(data->env, &tmp);
		else if (tmp->type == 8)
			ret += memory_shell(data, &tmp);
		else if (tmp->type == 0)
			ret += exec_shell(data, &tmp);
	}
	return (ret);
}
