/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 21:06:33 by daflynn           #+#    #+#             */
/*   Updated: 2025/06/10 09:40:03 by daflynn          ###   ########.fr       */
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

int	main(int argc, char *argv[], char *envp[])
{
	t_descriptor	descriptor;
	pid_t			pid1;
	pid_t			pid2;
	int				status;

	check_args(argc, argv);
	descriptor.fd_in = safe_open_infile(argv[1]);
	descriptor.fd_out = safe_open_outfile(argv[4], descriptor.fd_in);
	safe_pipe(descriptor.pipefd, descriptor.fd_in, descriptor.fd_out);
	pid1 = fork_and_execute(descriptor.fd_in, descriptor.pipefd[1], &descriptor,
			argv[2], envp);
	pid2 = fork_and_execute(descriptor.pipefd[0], descriptor.fd_out,
			&descriptor, argv[3], envp);
	closer(descriptor.fd_in, descriptor.fd_out, descriptor.pipefd[0],
		descriptor.pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}
