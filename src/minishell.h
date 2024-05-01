/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user-x <user-x@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 11:23:38 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/30 18:35:32 by user-x           ###   ########.fr       */
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
# include <limits.h>
# include <sys/mman.h>
# include "libft.h"


typedef struct s_env
{
	char			**envp;
	char			*name;
	char			*value;
	short			unsetted;
	struct s_env	*next;
}	t_env;

typedef struct s_memory
{
	char			*name;
	char			*value;
	struct s_memory	*next;
}	t_memory;

typedef struct s_line
{
	struct s_line	*parentesis;
	short			type;
	char			*line;
	short			logic;
	short			exit;
	struct s_line	*next;
}	t_line;

typedef struct s_data
{
	struct s_env	*env;
	struct s_line	*line;
	struct s_memory	*memory;
	int				*fds;
}	t_data;

extern int	g_ctrl;

t_env		*my_getenv(t_env *env, char *name);
short		ft_execve(char *str, t_env *env);
short		is_space(char c);
t_env		*env_lstlast(t_env *lst);
t_env		*env_lstnew(char *env, char **envp);
void		env_lstclear(t_env *lst);
void		env_lstadd_back(t_env **lst, t_env *new);
int			len_to_first_space(char *s);
int			find_char(char *s, char c);
int			header(void);
char		*remove_marks(char *line, t_memory *memory, t_env *env);
char		*f_strjoin(char *s1, char *s2);
void		handle_quote(char **tmp, int *i, char *line, char c);
char		*check_ref(char *tmp, t_memory *memory, t_env *env);
t_line		*line_lstnew(char *line, t_memory *memory, t_env *env);
char		*join_and_free_sec(char *s1, char *s2);
short		find_type(char *line);
short		set_logic(char *line);
int			skip_spaces(char *s);
void		line_lstclear(t_line *lst);
void		memory_lstclear(t_memory *lst);
t_memory	*memory_lstnew(char *name, char *value);
void		memory_lstadd_back(t_memory **lst, t_memory *new);
void		handle_logic(t_line **line);
int			mem_shell(t_data *data, t_line **line);
short		ret_of_chdir(char *cmd);
t_line		*ft_line_lstnew(char *line);
int			read_gnl(char **line);
char		**add_str_front_to_matrix(char **mtx, char *s);
int			pipex(t_data *data, char **argv);
t_env		*_getenv(t_env *env, char *name);
short		is_limiter_or_filename(int i, char *l, char c);
void		handle_limiter_or_filename(char **tmp, int *i, char *line, char c);
int			minishell(t_data *data, t_line *tmp, int ret, int ext);
int			pipe_shell(t_data *data, t_line **line);
int			cd_shell(t_data *data, t_line **line);
int			pwd_shell(t_line **line);
int			memory_shell(t_data *data, t_line **line);
int			export_shell(t_env **env, t_line **line);
int			unset_shell(t_env **env, t_line **line);
int			env_shell(t_env *env, t_line **line);
int			echo_shell(t_line **l, int ret);
void		handle_logic(t_line **line);
int			exec_shell(t_data *data, t_line **line);
t_line		*set_parentesis(char *line);
int			change_envpwd(t_env **env);
int			change_envopwd(t_env **env, char *opwd);
char		*remove_spaces(char *s);
void		cmd_not_found(char *cmd, short *res);
void		file_error(char *file);
int			ft_free(char *s);
char		*set_flag(char *s, int *j);
int			is_file(char *str);
int			n_flags_awk(char *str);
void		handle_awk_marks(char **tmp, int *i, char *line, char c);
char		*get_file(char *str, int i);
t_line		*new_empty_line(void);
char		*get_path(char *str, t_env *env);
const char	*prompt_location(t_env *env);
char		*get_name(char *s);
t_memory	*get_memory_node(t_memory *lst, char *name);
char		**get_cmds(char *line, int start, int end, int fd_in);
int			pipex_without_outfile(t_data *data, t_line **l, int i);
int			odd_quotes(char *line, char c);
short		nq(char *s, int times);
void		close_fds(int *fds);

#endif
