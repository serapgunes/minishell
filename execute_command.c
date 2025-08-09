//header

#include "minishell.h"

static int	cmd_skip_empty(char **args, int *cmd_idx)
{
	while (args[*cmd_idx] && args[*cmd_idx][0] == '\0')
		(*cmd_idx)++;
	if (!args[*cmd_idx])
	{
		if (*cmd_idx == 0)
			exit(1);
		exit(0);
	}
	return (0);
}

static void	run_builtin_or_exit(t_ast_tree *node, int cmd_idx, char ***envp, int in_pipeline)
{
	int	ret;

	ret = builtin(args_count(node->args + cmd_idx),
			node->args + cmd_idx, envp);
	if (ret != -1)
	{
		if (in_pipeline)
			signal(SIGPIPE, SIG_DFL);
		else
			signal(SIGPIPE, SIG_IGN);
		exit(ret);
	}
	signal(SIGPIPE, SIG_DFL);
}

static void exec_path(char *cmd, t_ast_tree *node, char ***envp, int cmd_idx)
{
	struct stat sb;

	if (stat(cmd, &sb) == -1)
	{
		if (errno == ENOENT)
		{
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": No such file or directory", 2);
		}
		else
			perror(cmd);
		exit(127);
	}
	if (S_ISDIR(sb.st_mode))
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": Is a directory", 2);
		exit(126);
	}
	if (access(cmd, X_OK) != 0)
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": Permission denied", 2);
		exit(126);
	}
	execve(cmd, node->args + cmd_idx, *envp);
	perror("execve");
	exit(1);
}

static void lookup_path(char *cmd, t_ast_tree *node, char ***envp, int cmd_idx)
{
	char *path;

	path = find_path(cmd);
	if (path)
	{
		execve(path, node->args + cmd_idx, *envp);
		free(path);
		perror("execve");               // stderr
		exit(1);
	}
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	exit(127);
}

void	execute_command(t_ast_tree *node, char ***envp, int in_pipeline)
{
	int		cmd_idx;
	char	*cmd;

    cmd_idx = 0;
	if (!node || !node->args)
		exit(1);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	cmd_skip_empty(node->args, &cmd_idx);
	cmd = node->args[cmd_idx];
	if (handle_redirections(node) != 0)
		exit(1);
	run_builtin_or_exit(node, cmd_idx, envp, in_pipeline);
	if (cmd[0] == '/' || cmd[0] == '.')
		exec_path(cmd, node, envp, cmd_idx);
	else
		lookup_path(cmd, node, envp, cmd_idx);
}
