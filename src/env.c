/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 08:57:13 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/26 10:22:05 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	short	handle_export_flag(t_env *env, char *line)
{
	int		i;

	if (line[0] != 'p')
	{
		write(2, "export: bad option: -", 22);
		return (write(2, &line[0], 1) - write(2, "\n", 1));
	}
	i = 1;
	while (line[i] && is_space(line[i]))
		i++;
	if (line[i])
	{
		write(2, "export: no such variable: ", 27);
		write(2, &line[i], len_to_first_space(&line[i]));
		return (write(2, "\n", 1) * 0);
	}
	while (env)
	{
		if (env->exported)
			printf("export %s=%s\n", env->name, env->value);
		env = env->next;
	}
	return (0);
}

short	handle_export(t_env **env, char *line)
{
	int		i;
	t_env	*last;

	i = 7;
	while (line[i] && is_space(line[i]))
		i++;
	if (!line[i])
		return (print_env(*env));
	if (line[i] == '-')
		return (handle_export_flag(*env, &line[i + 1]));
	if (find_char(&line[i], '=') > 0)
		f_lstadd_back(env, f_lstnew(&line[i]));
	last = f_lstlast(*env);
	last->exported = 1;
	return (0);
}

short	handle_unset(char *line, t_env **env)
{
	int		i;
	t_env	*temp;

	i = 6;
	while (line[i] && is_space(line[i]))
		i++;
	if (!line[i])
		return (6);
	temp = *env;
	while (temp)
	{
		if (!ft_strncmp(&line[i], temp->name, ft_strlen(temp->name))
			&& is_space(line[i + ft_strlen(temp->name)]))
		{
			temp->unsetted = 1;
			temp->exported = 0;
			return (0);
		}
		temp = temp->next;
	}
	return (0);
}

short	print_env(t_env *env)
{
	while (env)
	{
		if (env->unsetted == 0)
			printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
	return (0);
}
