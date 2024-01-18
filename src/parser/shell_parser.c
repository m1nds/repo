#include "shell_parser.h"

#include <stdlib.h>

enum parser_status parse_shell_command(struct ast **res, struct lexer *lexer)
{
    if (parse_rule_if(res, lexer) == PARSER_UNEXPECTED_TOKEN)
    {
        return PARSER_UNEXPECTED_TOKEN;
    }
    return PARSER_OK;
}

enum parser_status parse_rule_if(struct ast **res, struct lexer *lexer)
{
    struct lexer_token_save t = lexer_peek(lexer);
    enum token token = t.curr_tok;
    if (token != TOKEN_IF)
    {
        return PARSER_UNEXPECTED_TOKEN;
    }
    lexer_pop(lexer, true);
    struct ast *node_if = ast_new(NODE_IF, 3, NULL);
    struct ast *first_child = NULL;
    if (parse_compound_list(&first_child, lexer) == PARSER_UNEXPECTED_TOKEN)
    {
        ast_free(node_if);
        *res = NULL;
        print_error("Error while parsing first child of if", lexer);
        return PARSER_UNEXPECTED_TOKEN;
    }
    node_if->children[0] = first_child;
    t = lexer_peek(lexer);
    token = t.curr_tok;
    if (token != TOKEN_THEN)
    {
        ast_free(node_if);
        *res = NULL;
        print_error("Error while parsing then", lexer);
        return PARSER_UNEXPECTED_TOKEN;
    }
    lexer_pop(lexer, true);
    struct ast *second_child = NULL;
    if (parse_compound_list(&second_child, lexer) == PARSER_UNEXPECTED_TOKEN)
    {
        ast_free(node_if);
        *res = NULL;
        print_error("Error while parsing second child of if", lexer);
        return PARSER_UNEXPECTED_TOKEN;
    }
    node_if->children[1] = second_child;
    struct ast *third_child = NULL;
    if (parse_else_clause(&third_child, lexer) == PARSER_OK)
    {
        node_if->children[2] = third_child;
    }
    t = lexer_peek(lexer);
    token = t.curr_tok;
    *res = node_if;
    if (token == TOKEN_FI)
    {
        lexer_pop(lexer, true);
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}

enum parser_status parse_else_clause(struct ast **res, struct lexer *lexer)
{
    struct lexer_token_save t = lexer_peek(lexer);
    enum token token = t.curr_tok;
    if (token == TOKEN_ELSE)
    {
        lexer_pop(lexer, true);
        return parse_compound_list(res, lexer);
    }
    else if (token == TOKEN_ELIF)
    {
        lexer_pop(lexer, true);
        struct ast *node_if = ast_new(NODE_IF, 3, NULL);
        struct ast *first_child = NULL;
        if (parse_compound_list(&first_child, lexer) == PARSER_UNEXPECTED_TOKEN)
        {
            ast_free(node_if);
            *res = NULL;
            print_error("Error while parsing first child of elif", lexer);
            return PARSER_UNEXPECTED_TOKEN;
        }
        node_if->children[0] = first_child;
        t = lexer_peek(lexer);
        token = t.curr_tok;
        if (token != TOKEN_THEN)
        {
            ast_free(node_if);
            *res = NULL;
            print_error("Error while parsing then of elif", lexer);
            return PARSER_UNEXPECTED_TOKEN;
        }
        lexer_pop(lexer, true);
        struct ast *second_child = NULL;
        if (parse_compound_list(&second_child, lexer)
            == PARSER_UNEXPECTED_TOKEN)
        {
            ast_free(node_if);
            *res = NULL;
            print_error("Error while parsing second child of elif", lexer);
            return PARSER_UNEXPECTED_TOKEN;
        }
        node_if->children[1] = second_child;
        struct ast *third_child = NULL;
        if (parse_else_clause(&third_child, lexer) == PARSER_OK)
        {
            node_if->children[2] = third_child;
        }
        *res = node_if;
        return PARSER_OK;
    }
    else
    {
        return PARSER_UNEXPECTED_TOKEN;
    }
}