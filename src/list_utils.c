/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 09:21:04 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/26 10:21:43 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*f_lstnew(char *env)
{
	t_env	*new;
	int		i;

	i = 0;
	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	while (env[i] && env[i] != '=')
		i++;
	new->name = ft_substr(env, 0, i);
	new->value = ft_strdup(&env[i + 1]);
	new->unsetted = 0;
	new->exported = 1;
	new->next = NULL;
	return (new);
}

void	f_lstclear(t_env *lst)
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

t_env	*f_lstlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	f_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp = f_lstlast(*lst);
	temp->next = new;
}
