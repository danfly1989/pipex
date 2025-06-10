/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:28:28 by daflynn           #+#    #+#             */
/*   Updated: 2025/06/10 11:28:31 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

/*Ensure number of arguments is exactly five
as expected by pipex*/
void	check_args(int argc, char **argv)
{
	if (argc != 5)
	{
		ft_putstr_fd("Usage: ", STDERR_FILENO);
		ft_putstr_fd(argv[0], STDERR_FILENO);
		ft_putstr_fd(" file1 cmd1 cmd2 file2\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}

int	safe_open_infile(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open infile failed");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

int	safe_open_outfile(char *filename, int fd_in)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open outfile failed");
		close(fd_in);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void	safe_pipe(int pipefd[2], int fd_in, int fd_out)
{
	if (pipe(pipefd) < 0)
	{
		perror("pipe failed");
		close(fd_in);
		close(fd_out);
		exit(EXIT_FAILURE);
	}
}
