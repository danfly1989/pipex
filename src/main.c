/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:53:46 by daflynn           #+#    #+#             */
/*   Updated: 2025/06/10 16:53:52 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	closer(int fd_in, int fd_out, int pipefd1, int pipefd2)
{
	close(fd_in);
	close(fd_out);
	close(pipefd1);
	close(pipefd2);
}

static void	setup_param1(t_exec_params *params1, t_descriptor *desc,
		char **argv, char **envp)
{
	params1->input_fd = desc->fd_in;
	params1->output_fd = desc->pipefd[1];
	params1->desc = desc;
	params1->cmd_str = argv[2];
	params1->envp = envp;
}

static void	setup_param2(t_exec_params *params2, t_descriptor *desc,
		char **argv, char **envp)
{
	params2->input_fd = desc->pipefd[0];
	params2->output_fd = desc->fd_out;
	params2->desc = desc;
	params2->cmd_str = argv[3];
	params2->envp = envp;
}

static int	wait_and_return(pid_t pid1, pid_t pid2)
{
	int	status;

	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_descriptor	descriptor;
	t_exec_params	params1;
	t_exec_params	params2;
	pid_t			pid1;
	pid_t			pid2;

	check_args(argc, argv);
	descriptor.fd_in = safe_open_infile(argv[1]);
	descriptor.fd_out = safe_open_outfile(argv[4], descriptor.fd_in);
	safe_pipe(descriptor.pipefd, descriptor.fd_in, descriptor.fd_out);
	setup_param1(&params1, &descriptor, argv, envp);
	setup_param2(&params2, &descriptor, argv, envp);
	pid1 = fork_and_execute(&params1);
	pid2 = fork_and_execute(&params2);
	closer(descriptor.fd_in, descriptor.fd_out, descriptor.pipefd[0],
		descriptor.pipefd[1]);
	return (wait_and_return(pid1, pid2));
}
