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

typedef struct s_descriptor
{
	int	fd_in;
	int	fd_out;
	int	pipefd[2];
}		t_descriptor;

# ifndef IS_CHILD
#  define IS_CHILD 0
# endif

# ifndef FORK_FAILED
#  define FORK_FAILED -1
# endif

void	ft_free_split(char **split);
char	*find_path_in_env(char *cmd, char **envp);
int		safe_open_outfile(char *filename, int fd_in);

// Error handling functions
void	check_args(int argc, char **argv);
int		safe_open_infile(char *filename);

// Execution related functions
void	execute_command(char *cmd, char **envp);
char	**split_command(char *cmd);
void	handle_exec_error(char *path, char **args, int free_path);
void	handle_command_not_found(char **args);
void	execute_absolute_path(char **args, char **envp);
void	execute_from_path(char **args, char **envp);
void	safe_pipe(int pipefd[2], int fd_in, int fd_out);
pid_t	fork_and_execute(int input_fd, int output_fd, t_descriptor *desc,
			char *cmd, char **envp);

void	closer(int fd_in, int fd_out, int pipefd1, int pipefd2);

#endif
