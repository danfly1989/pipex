/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path_in_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:56:10 by daflynn           #+#    #+#             */
/*   Updated: 2025/06/10 10:56:14 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*search_dirs(char *cmd, char **dirs)
{
	int		i;
	char	*temp;
	char	*path;

	i = 0;
	while (dirs[i])
	{
		temp = ft_strjoin(dirs[i], "/");
		if (!temp)
			continue ;
		path = ft_strjoin(temp, cmd);
		free(temp);
		if (!path)
			continue ;
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

char	*find_path_in_env(char *cmd, char **envp)
{
	char	*path_env;
	char	**dirs;
	char	*result;

	path_env = get_path_env(envp);
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	result = search_dirs(cmd, dirs);
	ft_free_split(dirs);
	return (result);
}
