/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 11:20:15 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/25 09:00:26 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	header(void)
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
}

static	int	skip_spaces(char *line)
{
	int	i;

	i = 0;
	while (line[i] && is_space(line[i]))
		i++;
	return (i);
}

static	int	first_space_len(char *line)
{
	int	i;

	i = 0;
	while (line[i] && !is_space(line[i]))
		i++;
	return (i);
}

static	void	handle_err(short r, char *line)
{
	if (r < 1 || !first_space_len(line))
		return ;
	else if (r == 1)
	{
		write(2, "zsh: command not found: ", 25);
		write(2, line, first_space_len(line));
		write(2, "\n", 1);
	}
}

int	main(int argc, char **argv, char **env)
{
	char	*line;
	short	ret;

	(void)argc;
	(void)argv;
	if (ft_execve("clear", env))
		return (1);
	header();
	ret = 0;
	while (ret != -1)
	{
		line = readline("\001\033[1;92m\002Minishell% \001\033[0m\002");
		ret = ft_minishell(line, env);
		handle_err(ret, &line[skip_spaces(line)]);
		free(line);
	}
	return (0);
}
