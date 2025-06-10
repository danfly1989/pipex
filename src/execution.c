/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:53:31 by daflynn           #+#    #+#             */
/*   Updated: 2025/06/10 16:53:36 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	child_process_logic(t_exec_params *p)
{
	if (dup2(p->input_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 input failed");
		closer(p->desc->fd_in, p->desc->fd_out, p->desc->pipefd[0],
			p->desc->pipefd[1]);
		exit(EXIT_FAILURE);
	}
	if (dup2(p->output_fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 output failed");
		closer(p->desc->fd_in, p->desc->fd_out, p->desc->pipefd[0],
			p->desc->pipefd[1]);
		exit(EXIT_FAILURE);
	}
	closer(p->desc->fd_in, p->desc->fd_out, p->desc->pipefd[0],
		p->desc->pipefd[1]);
	execute_command(p->cmd_str, p->envp);
	exit(EXIT_FAILURE);
}

pid_t	fork_and_execute(t_exec_params *params)
{
	pid_t	pid;

	pid = fork();
	if (pid == FORK_FAILED)
	{
		perror("fork failed");
		closer(params->desc->fd_in, params->desc->fd_out,
			params->desc->pipefd[0], params->desc->pipefd[1]);
		exit(EXIT_FAILURE);
	}
	if (pid == IS_CHILD)
	{
		child_process_logic(params);
	}
	return (pid);
}

void	execute_command(char *cmd, char **envp)
{
	char	**args;

	args = split_command(cmd);
	if (ft_strchr(args[0], '/'))
		execute_absolute_path(args, envp);
	else
		execute_from_path(args, envp);
}
