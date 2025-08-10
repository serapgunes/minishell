/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:51 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/10 10:17:21 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int args_count(char **args)
{
	int i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int ft_exit_code(int temp)
{
	static int exit_code;

	if (temp == -1)
	{
		return (exit_code);
	}
	exit_code = temp;
	return (exit_code);
}

void ft_free(char **envp)
{
	int i = 0;

	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

void free_redir_list(t_redir *redir)
{
	t_redir *tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->type == HEREDOC && tmp->fd >= 0)
			close(tmp->fd); // heredoc fd'lerini de kapat
		if (tmp->target)
			free(tmp->target);
		free(tmp);
	}
}
void free_tokens(t_token *tokens)
{
	t_token *current;
	t_token *next;

	if (!tokens)
		return;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		free(current);
		current = next;
	}
}

void free_ast_tree(t_ast_tree *node)
{
	if (!node)
		return;
	if (node->args)
	{
		int i = 0;
		while (node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	if (node->redir_list)
	{
		free_redir_list(node->redir_list);
		node->redir_list = NULL;
	}
	if (node->left)
	{
		free_ast_tree(node->left);
		node->left = NULL;
	}
	if (node->right)
	{
		free_ast_tree(node->right);
		node->right = NULL;
	}
	free(node);
}

void cleanup(t_shell *shell, int mode)
{
	if (!shell)
		return;
	// if (shell->input)
	// 	free(shell->input);
	// if (shell->tokens)
	// 	free_tokens(shell->tokens);
	// if (shell->ast)
	// 	free_ast_tree(shell->ast);
	// if (shell->envp && mode == 0)
	// 	ft_free(shell->envp);
	// free(shell);
	if (shell->ast)
	{
		free_ast_tree(shell->ast);
		shell->ast = NULL;
	}
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->envp)
	{
		ft_free(shell->envp);
		shell->envp = NULL;
	}
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (mode == 0)
	{
		free(shell);
		// shell = NULL;
	}
}

int main(int argc, char **argv, char **env)
{
	t_shell *shell = malloc(sizeof(t_shell));

	if (!shell)
		return (1);
	shell->input = NULL;
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->envp = copy_env(env);
	if (!shell->envp)
	{
		free(shell);
		return (1);
	}

	(void)argv;
	(void)argc;
	ft_exit_code(0);
	while (1)
	{
		signal(SIGINT, signal_catch);
		shell->input = readline("minishell$ ");

		if (shell->input == NULL)
		{
			write(1, "exit\n", 5);
			cleanup(shell, 0);
			exit(0);
		}
		if (is_only_spaces(shell->input))
		{
			add_history(shell->input);
			cleanup(shell, 1);
			// shell = NULL;
			continue;
		}
		if (*shell->input)
			add_history(shell->input);

		shell->tokens = tokenize_input(shell->input);
		if (!shell->tokens)
		{
			ft_exit_code(0);
			cleanup(shell, 1);
			// shell = NULL;
			continue;
		}
		if (ft_parser(shell->tokens))
		{
			cleanup(shell, 1);
			// shell = NULL;
			continue;
		}
		shell->ast = ft_build_ast(shell->tokens, shell);
		shell->tokens = NULL;
		if (!shell->ast)
		{
			cleanup(shell, 1);
			// shell->ast = NULL;
			continue;
		}
		if (prepare_all_heredocs(shell->ast, shell) != 0)
		{
			cleanup(shell, 1);
			// shell = NULL;
			continue;
		}
		executor_structure(shell->ast, 0, shell);
		if (shell->ast)
		{
			free_ast_tree(shell->ast);
			shell->ast = NULL;
		}
		if (shell->input)
		{
			free(shell->input);
			shell->input = NULL;
		}
	}
	// ft_free(shell->envp);
	cleanup(shell, 0);
	return (ft_exit_code(-1));
}

// export yanlış çalışıyor
// minishell$ export VAR=42
// minishell$ env | grep VAR
// minishell$ export VAR2
// minishell$ export
// valgrind --leak-check=full --show-leak-kinds=all  --suppressions=./readline.supp ./minishell