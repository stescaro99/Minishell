/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 11:20:15 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/26 13:29:19 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	short	header(void)
{
	printf("\033[36m/-----------------------------------------------------");
	printf("-------------------------\\\n|                                  ");
	printf("                                            |\n|  ||\\  /||  || ");
	printf(" ||\\   ||  ||   ///////  ||    ||  |//////|  ||       ||       |");
	printf("\n|  ||\\\\//||  ||  ||\\\\  ||  ||  //        ||    ||  ||     ");
	printf("   ||       ||       |\n|  || \\/ ||  ||  || \\\\ ||  ||  \\\\");
	printf("\\\\\\\\\\   |//////|  |///|     ||       ||       |\n|  ||    ");
	printf("||  ||  ||  \\\\||  ||        //  ||    ||  ||        ||      ");
	printf(" ||       |\n|  ||    ||  ||  ||   \\||  ||  ///////   ||    ||");
	printf("  |\\\\\\\\\\\\|  |\\\\\\\\\\|  |\\\\\\\\\\|  |\n|             ");
	printf("                                                                 ");
	printf("|\n\\-----------------------------------------------------------");
	printf("-------------------/\n\033[0m\n");
	return (0);
}

static	t_env	*set_envlist(char **env)
{
	t_env	*envlist;
	t_env	*temp;
	int		i;

	i = 0;
	envlist = NULL;
	while (env[i])
	{
		temp = f_lstnew(env[i]);
		if (!temp)
			return (f_lstclear(envlist), NULL);
		f_lstadd_back(&envlist, temp);
		i++;
	}
	return (envlist);
}

static	char	*get_location(void)
{
	char	*cwd;
	char	*res;

	cwd = malloc(sizeof(char) * 1000);
	if (!cwd)
	{
		write(2, "Error!\nduring pwd", 17);
		return (NULL);
	}
	if (getcwd(cwd, 1000))
	{
		res = ft_strjoin("\001\033[1;94m\002~", cwd);
		//"\001\033[0m\002$ \001\033[0m\002"
		cwd = ft_strjoin(res, " \001\033[0m\002");
		free(res);
		return (cwd);
	}
	else
	{
		write(2, "Error!\nduring pwd", 17);
		return (free (cwd), NULL);
	}
	return (free(cwd), NULL);
}

int	main(int argc, char **argv, char **env)
{
	char	*line;
	short	ret;
	t_env	*envlist;
	char	*location;

	(void)argc;
	(void)argv;
	envlist = set_envlist(env);
	if (!envlist)
		return (1);
	if (ft_execve("clear", env))
		return (f_lstclear(envlist), 1);
	ret = header();
	while (ret != -1)
	{
		line = get_location();
		location = ft_strjoin("\001\033[1;92m\002Minishell:\001\033[0m\002",
				line);
		free(line);
		line = readline(location);
		free(location);
		ret = ft_minishell(line, &envlist, env);
		handle_err(ret, line);
	}
	return (f_lstclear(envlist), 0);
}
