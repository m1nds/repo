#ifndef AST_H
#define AST_H

#include <stddef.h>

enum ast_type
{
    NODE_COMMAND,
    NODE_SEMICOLON,
    NODE_IF,
    NODE_PIPE

};

struct ast
{
    enum ast_type node_type; // Type of node
    char **value; // The commande and it's argument the node contain NULL T
    struct ast **children; // list of the children of the node NULL T
};

/*
 * @def:   Create a new ast node and return it
 * @param: enum ast_type type: the type of node you want to create
 *         size_t nb_child: the number of child it should have
 *         char *value: the value of the first child set at ast->children[0]
 * @ret:   exit code or -1 if an error occurs
 * */
struct ast *ast_new(enum ast_type type, size_t nb_child, char *value);

/*
 * @def:   free the struct ast, all its children recursively and their **value
 * array
 * @param: struct ast *ast: the ast node that should be freed
 * @ret:   None
 * */
void ast_free(struct ast *ast);

/*
 * @def:   print on stdout the ast recursively on its children
 * @param: struct ast *ast: the ast node that should be printed
 * @ret:   None
 * */
void print_ast(struct ast *ast);

#endif /* ! AST_H */
