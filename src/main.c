/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 11:20:15 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/22 12:10:59 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_ctrl = 0;

int	header(void)
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
		temp = env_lstnew(env[i], env);
		if (!temp)
			return (env_lstclear(envlist), NULL);
		env_lstadd_back(&envlist, temp);
		i++;
	}
	return (envlist);
}

static	t_line	*set_line(t_memory *memory)
{
	char	*l;
	t_line	*line;

	l = readline("\033[1;92mMinishell$ \033[0m");
	if (!l)
	{
		printf("\n");
		return (NULL);
	}
	line = line_lstnew(l, memory);
	add_history(l);
	free(l);
	return (line);
}

static	void	signal_handler(int signum)
{
	if (signum == SIGINT)
		g_ctrl = 1;
	else if (signum == SIGQUIT)
		g_ctrl = 2;
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	int		ret;

	(void)argc;
	(void)argv;
	data.env = set_envlist(env);
	ret = WEXITSTATUS(ft_execve("clear", data.env));
	data.memory = NULL;
	while (ret < 9)
	{
		signal(SIGQUIT, signal_handler);
		if (g_ctrl == 2 && printf("\033[1;92mMinishell$ \033[0mexit\n"))
			break ;
		signal(SIGINT, signal_handler);
		data.line = set_line(data.memory);
		if (g_ctrl == 1 && printf("^C\n"))
			line_lstclear(data.line);
		if (g_ctrl == 2 && g_ctrl--)
			continue ;
		ret += minishell(&data, data.line, 0, 10);
		line_lstclear(data.line);
	}
	env_lstclear(data.env);
	memory_lstclear(data.memory);
	return (clear_history(), 0);
}
