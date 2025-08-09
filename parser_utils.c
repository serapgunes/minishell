/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segunes <segunes@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 20:54:37 by sakdil            #+#    #+#             */
/*   Updated: 2025/08/10 02:17:16 by segunes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int find_pipe_split(t_token *tokens,
                           t_token **left, t_token **mid, t_token **right)
{
    t_token *prev = NULL;

    *left = tokens;
    *mid = NULL;
    *right = NULL;

    while (tokens)
    {
        if (tokens->type == PIPE)
        {
            if (!tokens->next) // pipe'tan sonra hiçbir şey yoksa: syntax error
                return (1);

            *mid = tokens;         // pipe düğümünü geri ver
            *right = tokens->next; // sağ liste başlangıcı

            tokens->next = NULL; // pipe'ı sağdan kopar
            if (prev)
                prev->next = NULL; // solu da pipe'tan kopar

            return (0);
        }
        prev = tokens;
        tokens = tokens->next;
    }
    return (0);
}

static int count_words(t_token *cur)
{
    int count;

    count = 0;
    while (cur)
    {
        if (cur->type == WORD)
            count++;
        cur = cur->next;
    }
    return (count);
}

static t_ast_tree *build_cmd_node(t_token *left_token)
{
    t_ast_tree *node;
    char **args;
    int count = count_words(left_token);

    node = create_cmd_node_with_args(count, &args);
    if (!node)
        return NULL;

    if (fill_cmd_from_tokens(node, left_token, args) != 0)
    {
        free(args);
        free(node);
        free_token_list(left_token);
        return NULL;
    }
    node->args = args;
    free_token_list(left_token);
    return node;
}

static t_ast_tree *make_pipe_node(t_token *left, t_token *right, t_token *mid, t_shell *shell)
{
    t_ast_tree *node;

    node = malloc(sizeof(t_ast_tree));
    if (!node)
        return (NULL);
    node->type = NODE_PIPE;
    node->redir_list = NULL;
    node->args = NULL;
    node->left = ft_build_ast(left, shell);
    node->right = ft_build_ast(right, shell);
    if (mid)
    {
        if (mid->value)
            free(mid->value);
        free(mid);
    }
    if (!node->left || !node->right)
    {
        printf("syntax error near unexpected token\n");
        free(node);
        return (NULL);
    }
    return (node);
}

t_ast_tree *ft_build_ast(t_token *tokens, t_shell *shell)
{
    t_token *left, *right, *mid;

    if (!tokens)
        return (NULL);

    if (find_pipe_split(tokens, &left, &mid, &right))
    {
        printf("syntax error near unexpected token\n");
        // HATA: bütün token zinciri tek parça olabilir; tamamını bırak
        free_token_list(tokens);
        return (NULL);
    }
    if (mid) // pipe bulundu
        return make_pipe_node(left, right, mid, shell);

    // pipe yoksa tek komut; build_cmd_node tokenları kendi içinde free ediyor
    return (build_cmd_node(tokens));
}
