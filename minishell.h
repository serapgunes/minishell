/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil < sakdil@student.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:47 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/12 11:25:13 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <fcntl.h>
# include "libft/libft.h"
# include <signal.h>
# include <errno.h>
# include <sys/stat.h>

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIR
}	t_node_type;

typedef struct s_redir
{
	int				type;     // REDIR_IN, etc.
	char			*target; // e.g. "input.txt"
	int				fd;       // HEREDOC için ekeldik
	int 			quoted;
	struct s_redir	*next;
}	t_redir;

typedef struct s_ast_tree
{
	t_node_type			type;
	char				**args;
	t_redir				*redir_list;
	struct s_ast_tree	*left; // her nodun içinde alt nodelar var bu alt nodelar aynı yapıya yani structa eşit olduğu için böyle
	struct s_ast_tree	*right;
}	t_ast_tree;

typedef enum e_token_type
{
    WORD,
    PIPE,      // |
    REDIR_OUT, // >
    REDIR_IN,  // <
    APPEND,    // >>
    HEREDOC,   // <<
    INVALID
} t_token_type;

typedef struct s_token
{
    t_token_type type;
    char *value;
    int  quoted;
    struct s_token *next;
} t_token;

typedef struct s_shell
{
    char *input;
    t_token *tokens;
    t_ast_tree *ast;
    char **envp;
} t_shell;

typedef struct	s_std
{
	int	std_in;
	int	std_out;
}	t_std;

typedef struct s_rtok {
	char	**envp;
	int		*has_quote;
	int		expand;
}	t_rtok;


void cleanup(t_shell *shell, int mode); // düşün

int builtin_cd(int argc, char **argv);
int builtin_echo(int argc, char **argv);
int control_env(int argc);
int builtin_env(char ***env);
int builtin_exit(int argc, char **argv, t_shell *shell);
int is_valid_identifier(const char *name);
int builtin_export(int argc, char **argv, char ***envp);
int find_in_environ(const char *name, char **env);
int extend_env(char ***envp, char *new_entry);
int set_env_var(char *arg, char ***envp);
void print_sorted_env(char **env);
void print_invalid_identifier(char *arg);
int builtin_pwd(void);
int builtin_unset(int argc, char **argv, char ***env);
int builtin(int argc, char **argv, char ***env, t_shell *shell);
int is_builtin(char *cmd);
int is_digit(int c);

int is_only_spaces(char *str);
void executor_structure(t_ast_tree *node, int in_pipeline, t_shell *shell);

// parser///////////////////////
t_ast_tree *ft_build_ast(t_token *tokens, t_shell *shell);
// void print_ast(t_ast_tree *node, int depth);
int ft_parser(t_token *input);
////////////////////////////////

// path_find//////////////////////////
char *find_path(char *command, char **envp);
//////////////////////////////////////

// signal/////////////////////////
void signal_catch(int signum);
int check_sigint_flag(void);
void signal_child(int signum);
/////////////////////////////////

int args_count(char **args);
char *ft_charjoin_free(char *res, char *val, int flag);
char *ft_charjoin(char *res, char c);
void free_tokens(t_token *head); // şu anki mainde yok
t_token *create_word_token(char *value);
t_token *create_pipe_token(char *value);
t_token *create_redir_token(char *value);
t_token *create_token(char *value);
int is_word_char(char c);
void add_token_to_list(t_token **head, t_token *new);
int handle_redir_operator(char *s, t_token **head);

char *normalize_filename(char *str);

// exit code
int ft_exit_code(int temp);
char **copy_env(char **env);
int handle_redirections(t_ast_tree *node);
void execute_command(t_ast_tree *node, int in_pipeline, t_shell *shell);
void execute_pipe(t_ast_tree *node, t_shell *shell);
t_ast_tree *create_cmd_node_with_args(int count, char ***args_out);
int fill_cmd_from_tokens(t_ast_tree *node, t_token *current, char **args);
char *process_quoted(char *s, int *i, char quote_char);
int prepare_all_heredocs(t_ast_tree *node, t_shell *shell);
void free_token_list(t_token *cur); // bak

void	exit_with_cmd_error(char *cmd, char *msg, int code, t_shell *shell);
void	restore_std(t_std std);
void	free_ast_tree(t_ast_tree *node);
void	ft_free(char **envp);


const char *ms_getenv(const char *name, char **envp);
t_token *tokenize_input(char *input, char **envp);
char   *expand_variable(char *str, char **envp);
char   *collect_argument(char *input, int *i, char **envp);
int     handle_redir_file(char *s, int *i, t_token **head, char **envp);
char   *process_unquoted(char *s, int *j, char **envp);
int	is_redir_separator(char c);
void	signal_heredoc(int sig);
void	redir_push_back(t_redir **list, t_redir *r);
char	*expand_double_quote_content(char *input, int start, int end, char **envp);
t_token	*last_token(t_token *head);
char	*process_unquoted_raw(char *s, int *j);
int	prev_is_heredoc(t_token *head);

#endif
