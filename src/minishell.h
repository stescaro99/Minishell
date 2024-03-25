/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 11:23:38 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/25 11:44:43 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

short	ft_execve(char *str, char **env);
short	ft_minishell(char *line, char **env);
short	is_space(char c);
short	is_pipe(char *line);
short	print_env(char **env);
void	print_color(char *str, char *color);
short	handle_echo(char *line, short q, short f);
char	*echo_readline(char *prompt);

#endif
