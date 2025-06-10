/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:12:44 by daflynn           #+#    #+#             */
/*   Updated: 2025/06/10 11:13:00 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex.h"

/* Helper Functions */
char	**split_command(char *cmd)
{
	char	**args;

	args = ft_split(cmd, ' ');
	if (!args)
	{
		perror("ft_split failed");
		exit(EXIT_FAILURE);
	}
	return (args);
}

void	handle_exec_error(char *path, char **args, int free_path)
{
	perror("execve failed");
	if (free_path)
		free(path);
	ft_free_split(args);
	exit(EXIT_FAILURE);
}

void	handle_command_not_found(char **args)
{
	ft_putstr_fd("Command not found: ", STDERR_FILENO);
	ft_putstr_fd(args[0], STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	ft_free_split(args);
	exit(127);
}

void	execute_absolute_path(char **args, char **envp)
{
	execve(args[0], args, envp);
	handle_exec_error(args[0], args, 0);
}

void	execute_from_path(char **args, char **envp)
{
	char	*path;

	path = find_path_in_env(args[0], envp);
	if (!path)
		handle_command_not_found(args);
	execve(path, args, envp);
	handle_exec_error(path, args, 1);
}
