/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 11:23:38 by stescaro          #+#    #+#             */
/*   Updated: 2024/03/26 11:52:26 by stescaro         ###   ########.fr       */
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

typedef struct s_env
{
	char			*name;
	char			*value;
	short			exported;
	short			unsetted;
	struct s_env	*next;
}	t_env;

short	ft_execve(char *str, char **env);
short	ft_minishell(char *line, t_env **env, char **envp);
short	is_space(char c);
short	is_pipe(char *line);
short	print_env(t_env	*env);
void	print_color(char *str, char *color);
short	handle_echo(char *line, short q, short f);
char	*echo_readline(char *prompt);
short	handle_unset(char *line, t_env **env);
short	handle_pwd(char *line);
short	handle_cd(char *line);
void	handle_err(short r, char *line);
t_env	*f_lstlast(t_env *lst);
t_env	*f_lstnew(char *env);
void	f_lstclear(t_env *lst);
void	f_lstadd_back(t_env **lst, t_env *new);
int		len_to_first_space(char *s);
int		find_char(char *s, char c);
short	handle_export(t_env **env, char *line);
void	skip_printf(char *line);

#endif
