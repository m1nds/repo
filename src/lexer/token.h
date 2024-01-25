#ifndef TOKEN_H
#define TOKEN_H

enum token
{
    TOKEN_NONE = -1,
    TOKEN_IF,
    TOKEN_FI,
    TOKEN_ELSE,
    TOKEN_THEN,
    TOKEN_ELIF,
    TOKEN_WHILE,
    TOKEN_UNTIL,
    TOKEN_FOR,
    TOKEN_IN,
    TOKEN_DO,
    TOKEN_DONE,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_NOT,
    TOKEN_ASSIGNMENT,
    TOKEN_ESCAPED_WORD,
    TOKEN_WORD,
    TOKEN_EOF = 100,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_SEMICOLON,
    TOKEN_LEFT_PARENTHESES,
    TOKEN_RIGHT_PARENTHESES,
    TOKEN_RETURN,
    TOKEN_STDIN,
    TOKEN_IO_NUMBER,
    TOKEN_REDIR_PIPE,
    TOKEN_REDIR_STDIN,
    TOKEN_REDIR_STDOUT,
    TOKEN_REDIR_APPEND,
    TOKEN_REDIR_DUPIN,
    TOKEN_REDIR_DUPOUT,
    TOKEN_REDIR_INOUT,
    //......
};

#endif /* TOKEN_H */
