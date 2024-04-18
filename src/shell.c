/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 09:22:32 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/09 09:52:40 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_logic(t_line **line)
{
	int		logic;
	t_line	*temp;

	temp = *line;
	logic = temp->logic;
	if (logic == 1 && temp->exit > 0)
		temp = temp->next;
	else if (logic == 1 && temp->exit < 1 && temp->next
		&& temp->next->logic != 1)
		temp->next->logic = 3;
	else if ((logic == 2 || logic == 3) && temp->exit < 1)
	{
		while (temp && temp->next)
			temp = temp->next;
	}
	temp = temp->next;
}

int	echo_shell(t_data *data, t_line **line)
{
	int		i;
	char	*tmp;
	int		flag;

	i = 4;
	tmp = (*line)->line;
	flag = 0;
	if (!tmp[i])
	{
		printf("\n");
		return (handle_logic(line), 0);
	}
	i++;
	if (tmp[i] == '-' && tmp[i + 1] && tmp[i + 1] == 'n'
		&& is_space(tmp[i + 2]) && flag++)
		i += 3;
	if (!tmp[i - 1] || !tmp[i])
		return (handle_logic(line), 0);
	(*line)->exit = printf("%s", &tmp[i]);
	if (flag)
		printf("\e[38;2;0;0;0m\e[48;2;255;255;255m%%\033[0m");
	printf("\n");
	handle_logic(line);
	return (0);
}

int	memory_shell(t_data *data, t_line **line)
{
	char		*l;
	char		*name;
	char		*value;
	int			i;

	l = (*line)->line;
	i = find_char(l, '=');
	if (i < 1 || is_space(l[i - 1]) || is_space(l[i + 1]) || !l[i + 1])
	{
		(*line)->exit -= 2;
		return (handle_logic(line), -1);
	}
	name = ft_substr(l, 0, i);
	value = ft_substr(l, i + 1, ft_strlen(l) - i - 1);
	memory_lstadd_back(&(data->memory), memory_lstnew(name, value));
	handle_logic(line);
	free(value);
	return (free(name), 0);
}

int	cd_shell(t_data *data, t_line **line)
{
	char	*tmp;
	char	*cmd;
	int		i;

	tmp = (*line)->line;
	i = 2;
	if (!tmp[i])
	{
		chdir(getenv("HOME"));
		change_envpwd(&data->env);
		return (handle_logic(line), 0);
	}
	cmd = ft_calloc(sizeof(char), 1000);
	if (!cmd)
	{
		(*line)->exit -= 2;
		return (handle_logic(line), -1);
	}
	ft_strlcpy(cmd, &tmp[i], len_to_first_space(&tmp[i]));
	if (chdir(cmd) && write(2, "cd: no such file or directory: ", 32))
	{
		(*line)->exit -= (write(2, cmd, ft_strlen(cmd)) + write(2, "\n", 1));
		return (free(cmd), handle_logic(line), -1);
	}
	return (handle_logic(line), free(cmd), change_envpwd(&data->env));
}

int	pwd_shell(t_data *data, t_line **line)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	if (!tmp)
	{
		(*line)->exit -= 2;
		return (handle_logic(line), -1);
	}
	(*line)->exit = printf("%s\n", tmp);
	handle_logic(line);
	free(tmp);
	return (0);
}
