/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:02:15 by daflynn           #+#    #+#             */
/*   Updated: 2025/06/10 11:02:34 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

pid_t	fork_and_execute(int input_fd, int output_fd, t_descriptor *desc,
		char *cmd, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == FORK_FAILED)
	{
		perror("fork failed");
		closer(desc->fd_in, desc->fd_out, desc->pipefd[0], desc->pipefd[1]);
		exit(EXIT_FAILURE);
	}
	if (pid == IS_CHILD)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 input failed");
			closer(desc->fd_in, desc->fd_out, desc->pipefd[0], desc->pipefd[1]);
			exit(EXIT_FAILURE);
		}
		if (dup2(output_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 output failed");
			closer(desc->fd_in, desc->fd_out, desc->pipefd[0], desc->pipefd[1]);
			exit(EXIT_FAILURE);
		}
		closer(desc->fd_in, desc->fd_out, desc->pipefd[0], desc->pipefd[1]);
		execute_command(cmd, envp);
		exit(EXIT_FAILURE);
	}
	return (pid);
}

/* Main Function */
void	execute_command(char *cmd, char **envp)
{
	char	**args;

	args = split_command(cmd);
	if (ft_strchr(args[0], '/'))
		execute_absolute_path(args, envp);
	else
		execute_from_path(args, envp);
}
