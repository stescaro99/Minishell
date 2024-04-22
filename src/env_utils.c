/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 09:21:04 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/18 11:00:52 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	memory_lstadd_back(t_memory **lst, t_memory *new)
{
	t_memory	*temp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

int	change_envpwd(t_env **env)
{
	t_env	*temp;

	temp = *env;
	while (temp)
	{
		if (!ft_strncmp(temp->name, "PWD", 4))
		{
			free(temp->value);
			temp->unsetted = 0;
			temp->value = ft_strdup(getenv("PWD"));
			return (0);
		}
		temp = temp->next;
	}
	return (-1);
}

t_env	*env_lstnew(char *env)
{
	t_env	*new;
	int		i;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	i = find_char(env, '=');
	if (i > 0)
	{
		new->name = ft_substr(env, 0, i);
		new->value = ft_strdup(&env[i + 1]);
	}
	else
	{
		new->name = ft_strdup(env);
		new->value = NULL;
	}
	new->unsetted = 0;
	new->exported = 1;
	new->next = NULL;
	return (new);
}

void	env_lstclear(t_env *lst)
{
	t_env	*temp;

	while (lst)
	{
		temp = lst->next;
		free(lst->name);
		free(lst->value);
		free(lst);
		lst = temp;
	}
}

t_env	*env_lstlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
