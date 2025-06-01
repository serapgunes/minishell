/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:40:47 by sakdil            #+#    #+#             */
/*   Updated: 2025/06/01 19:14:15 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <unistd.h> 
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "libft/libft.h"

extern int	g_last_status;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_export_data
{
    char *arg;
    char *eq;
    int status;
    int i;
} t_export_data;

typedef enum e_token_type
{
    WORD,
    PIPE, // |
    REDIR_OUT, // >
    REDIR_IN, // <
    APPEND, // >>
    HEREDOC, // <<
    INVALID
} t_token_type;

typedef struct s_token
{
	t_token_type type;
	char *value;
    struct s_token *next;
} t_token;


void		execute_command(char *input);
int			is_only_spaces(char *str);
const char *pathname(char *command);
int 		builtin(int argc, char **argv, char **env, t_list *history);
void		add_to_history(t_list **history, char *input);
void		print_history(t_list *history);
int builtin_echo(int argc, char **argv, char **env);
int	builtin_cd(int argc, char **argv);
int builtin_env(char **env);
int	builtin_exit(int argc, char **argv);
int control_env(int argc);
int builtin_export(int argc, char **argv);
int builtin_export_process(int argc, char **argv, int status, int i);
int builtin_pwd(void);
int builtin_unset(int argc, char **argv);
int is_valid_identifier(const char *name);
int find_in_environ(const char *name);
// void print_tokens(t_token *head);


t_token *tokenize_input(char *input);
t_token *create_token(char *value);
t_token *create_redir_token(char *value);
t_token *create_pipe_token(char *value);
t_token *create_word_token(char *value);
void	free_tokens(t_token *head);
char	*expand_variable(char *str);


#endif