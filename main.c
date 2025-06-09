/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 21:06:33 by daflynn           #+#    #+#             */
/*   Updated: 2025/06/09 21:07:50 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// You'll need to implement or include these:
char	**ft_split(char *str, char sep);
void	free_split(char **arr);

char	*find_path_in_env(char *cmd, char **envp)
{
	char	*path_env = NULL;
	for (int i = 0; envp[i]; i++)
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break;
		}
	}
	if (!path_env)
		return NULL;

	char	**dirs = ft_split(path_env, ':');
	if (!dirs)
		return NULL;

	for (int i = 0; dirs[i]; i++)
	{
		char *path = malloc(strlen(dirs[i]) + strlen(cmd) + 2);
		if (!path)
			continue;
		sprintf(path, "%s/%s", dirs[i], cmd);
		if (access(path, X_OK) == 0)
		{
			free_split(dirs);
			return path;
		}
		free(path);
	}
	free_split(dirs);
	return NULL;
}

void	execute_command(char *cmd, char **envp)
{
	char **args = ft_split(cmd, ' ');
	if (!args)
		exit(EXIT_FAILURE);

	if (strchr(args[0], '/'))
	{
		execve(args[0], args, envp);
		perror("execve failed");
		free_split(args);
		exit(EXIT_FAILURE);
	}

	char *path = find_path_in_env(args[0], envp);
	if (!path)
	{
		fprintf(stderr, "Command not found: %s\n", args[0]);
		free_split(args);
		exit(127);
	}

	execve(path, args, envp);
	perror("execve failed");
	free(path);
	free_split(args);
	exit(EXIT_FAILURE);
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc != 5)
	{
		fprintf(stderr, "Usage: %s file1 cmd1 cmd2 file2\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int fd_in = open(argv[1], O_RDONLY);
	if (fd_in < 0)
	{
		perror("open infile failed");
		exit(EXIT_FAILURE);
	}

	int fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out < 0)
	{
		perror("open outfile failed");
		close(fd_in);
		exit(EXIT_FAILURE);
	}

	int pipefd[2];
	if (pipe(pipefd) < 0)
	{
		perror("pipe failed");
		close(fd_in);
		close(fd_out);
		exit(EXIT_FAILURE);
	}

	pid_t pid1 = fork();
	if (pid1 < 0)
	{
		perror("fork1 failed");
		close(fd_in);
		close(fd_out);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(EXIT_FAILURE);
	}
	if (pid1 == 0)
	{
		dup2(fd_in, STDIN_FILENO);
		dup2(pipefd[1], STDOUT_FILENO);
		close(fd_in);
		close(fd_out);
		close(pipefd[0]);
		close(pipefd[1]);
		execute_command(argv[2], envp);
	}

	pid_t pid2 = fork();
	if (pid2 < 0)
	{
		perror("fork2 failed");
		close(fd_in);
		close(fd_out);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(EXIT_FAILURE);
	}
	if (pid2 == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_in);
		close(fd_out);
		close(pipefd[0]);
		close(pipefd[1]);
		execute_command(argv[3], envp);
	}

	// Parent process
	close(fd_in);
	close(fd_out);
	close(pipefd[0]);
	close(pipefd[1]);

	int status;
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);

	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	return 0;
}

