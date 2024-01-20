#define _POSIX_C_SOURCE 200809L
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exec/exec.h"
#include "input/input.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

int main(int argc, char **argv)
{
    int err = 0;
    int options = 0;
    FILE *input = get_input(argc, argv, &options, &err);
    if (!input)
    {
        switch (err)
        {
        case 2:
            errx(2, "option requires an argument");
            break;

        case 127:
            errx(127, "No such file or directory");
            break;

        default:
            break;
        }
    }
    struct lexer *lexer = create_lexer(input);
    /*enum token token = TOKEN_NONE;
    while (token != TOKEN_EOF)
    {
        token = lexer_peek(lexer).curr_tok;
        char *string = lexer_peek(lexer).tok_str;
        printf("Token : %i, String : %s\n", token, string);
        lexer_pop(lexer, true);
    }
    printf("\n");
    */
    struct ast *ast = NULL;
    enum parser_status status = parse_input(&ast, lexer);
    if (check_prettyprint(&options))
    {
        print_ast(ast);
    }
    int return_code = 0;
    while (status == PARSER_OK)
    {
        if (ast != NULL)
        {
            return_code = evaluate_ast(ast);
        }
        ast_free(ast);
        enum token token = lexer_peek(lexer).curr_tok;
        if (token == TOKEN_EOF)
        {
            break;
        }
        ast = NULL;
        fflush(NULL);
        status = parse_input(&ast, lexer);
        // printf("Parser is OK!\n");
    }
    if (status == PARSER_UNEXPECTED_TOKEN)
    {
        fprintf(stderr, "Parser is not ok :(\n");
        ast_free(ast);
        free_lexer(lexer);
        fclose(input);
        return 2;
    }
    free_lexer(lexer);
    fclose(input);
    return return_code;
}
