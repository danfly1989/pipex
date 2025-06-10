/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 09:43:46 by daflynn           #+#    #+#             */
/*   Updated: 2025/06/10 09:49:09 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <sys/wait.h>

# define FORK_FAILED -1
# define IS_CHILD 0

typedef struct s_descriptor
{
	int				fd_in;
	int				fd_out;
	int				pipefd[2];
}					t_descriptor;

typedef struct s_exec_params
{
	int				input_fd;
	int				output_fd;
	t_descriptor	*desc;
	char			*cmd_str;
	char			**envp;
}					t_exec_params;

pid_t				fork_and_execute(t_exec_params *params);
void				execute_command(char *cmd, char **envp);

char				**split_command(char *cmd);
void				handle_exec_error(char *path, char **args, int free_path);
void				handle_command_not_found(char **args);
void				execute_absolute_path(char **args, char **envp);
void				execute_from_path(char **args, char **envp);

void				closer(int fd_in, int fd_out, int pipefd1, int pipefd2);
int					safe_open_infile(char *filepath);
int					safe_open_outfile(char *filepath, int infile_fd);
void				safe_pipe(int *pipefd, int fd_in, int fd_out);

char				*find_path_in_env(char *cmd_name, char **envp);
char				*ft_strchr(const char *s, int c);
char				**ft_split(char const *s, char c);
void				ft_putstr_fd(char *s, int fd);
void				ft_putchar_fd(char c, int fd);
void				ft_free_split(char **split_array);
void				check_args(int argc, char *argv[]);

#endif
