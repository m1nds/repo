#include "lexer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct lexer *create_lexer(FILE *input)
{
    struct lexer *result = calloc(1, sizeof(struct lexer));
    // Check for NULL
    if (result == NULL)
    {
        return NULL;
    }
    result->input = input;
    result->pos = ftell(input);
    return result;
}

void free_lexer(struct lexer *lexer)
{
    if (lexer != NULL)
    {
        free(lexer);
    }
}

static enum token match_word(char *word)
{
    if (strcmp(word, "if") == 0)
    {
        return TOKEN_IF;
    }
    if (strcmp(word, "then") == 0)
    {
        return TOKEN_THEN;
    }
    if (strcmp(word, "elif") == 0)
    {
        return TOKEN_ELIF;
    }
    if (strcmp(word, "else") == 0)
    {
        return TOKEN_ELSE;
    }
    if (strcmp(word, "fi") == 0)
    {
        return TOKEN_FI;
    }
    return TOKEN_WORD;
}

/*
 * Returns true if the current character belongs in the word
 * False if not
 */
static bool is_continuous_word(char character)
{
    return character != ' ' && character != ';' && character != '\n'
        && character != '\0' && character != EOF;
}

static void ignore_line(struct lexer *lexer)
{
    char c = fgetc(lexer->input);
    lexer->pos++;
    while (c != '\n' && c != EOF && c != '\0')
    {
        c = fgetc(lexer->input);
        lexer->pos++;
    }
    if (c == '\n')
    {
        lexer->pos++;
    }
    else
    {
        ungetc(c, lexer->input);
    }
}

static enum token check_single_quote(struct lexer *lexer)
{
    char c = fgetc(lexer->input);
    while (c != '\'')
    {
        if (c == EOF || c == '\0')
        {
            fseek(lexer->input, lexer->pos, SEEK_SET);
            return TOKEN_STDIN;
        }
        c = fgetc(lexer->input);
    }
    fseek(lexer->input, lexer->pos, SEEK_SET);
    return TOKEN_WORD;
}

static enum token get_next_token(struct lexer *lexer)
{
    char c = fgetc(lexer->input);
    // Remove all spaces before the word
    while (c == ' ')
    {
        c = fgetc(lexer->input);
        lexer->pos++;
    }
    if (c == '#')
    {
        ignore_line(lexer);
        c = fgetc(lexer->input);
    }
    // Not sure if \0 should count as EOF but it should be fine
    if (c == EOF || c == '\0')
    {
        return TOKEN_EOF;
    }
    if (c == ';')
    {
        ungetc(c, lexer->input);
        return TOKEN_SEMICOLON;
    }
    if (c == '\n')
    {
        ungetc(c, lexer->input);
        return TOKEN_RETURN;
    }
    if (c == '\'')
    {
        // Check if the single quote is closed or not
        return check_single_quote(lexer);
    }
    // Special case if there is a \n after \, need to check later
    if (c == '\\')
    {
        ungetc(c, lexer->input);
        return TOKEN_WORD;
    }
    char *word = calloc(100, sizeof(char));
    size_t word_pos = 0;
    // Very basic version, won't work later
    while (is_continuous_word(c))
    {
        word[word_pos++] = c;
        c = fgetc(lexer->input);
        // Failsafe if a word is too big
        if (word_pos >= 100)
        {
            return TOKEN_WORD;
        }
    }
    fseek(lexer->input, lexer->pos, SEEK_SET);
    enum token result = match_word(word);
    free(word);
    return result;
}

enum token lexer_peek(struct lexer *lexer)
{
    return get_next_token(lexer);
}

static void skip_single_quote(struct lexer *lexer)
{
    lexer->pos++;
    char c = fgetc(lexer->input);
    while (c != '\'')
    {
        c = fgetc(lexer->input);
        lexer->pos++;
    }
    lexer->pos++;
}

enum token lexer_pop(struct lexer *lexer)
{
    enum token token = get_next_token(lexer);
    char c = fgetc(lexer->input);
    // Update position of lexer
    if (token == TOKEN_WORD && c == '\'')
    {
        skip_single_quote(lexer);
        return token;
    }
    while (is_continuous_word(c))
    {
        c = fgetc(lexer->input);
        lexer->pos++;
    }
    if (token == TOKEN_RETURN || token == TOKEN_SEMICOLON)
    {
        lexer->pos++;
    }
    else
    {
        ungetc(c, lexer->input);
    }
    return token;
}

char *handle_single_quote(struct lexer *lexer, char *buffer)
{
    size_t buffer_pos = 0;
    char c = fgetc(lexer->input);
    while (c != '\'')
    {
        buffer[buffer_pos++] = c;
        c = fgetc(lexer->input);
        if (c == EOF || c == '\0')
        {
            free(buffer);
            return "no end to single quote";
        }
        if (buffer_pos % 100 == 0)
        {
            buffer = realloc(buffer, (buffer_pos + 100) * sizeof(char));
        }
    }
    buffer[buffer_pos] = '\0';
    fseek(lexer->input, lexer->pos, SEEK_SET);
    return buffer;
}

char *get_token_string(struct lexer *lexer)
{
    enum token token = lexer_peek(lexer);
    if (token != TOKEN_WORD)
    {
        return token_string(token);
    }
    // Read the input again and store it in a buffer
    char *buffer = malloc(100 * sizeof(char));
    size_t buffer_pos = 0;
    char c = fgetc(lexer->input);
    if (c == '\'')
    {
        return handle_single_quote(lexer, buffer);
    }
    if (c == '\\')
    {
        c = fgetc(lexer->input);
    }
    while (is_continuous_word(c))
    {
        buffer[buffer_pos++] = c;
        c = fgetc(lexer->input);
        // If the buffer is too small, we increase it by 100
        if (buffer_pos % 100 == 0)
        {
            buffer = realloc(buffer, (buffer_pos + 100) * sizeof(char));
        }
    }
    buffer[buffer_pos] = '\0';
    fseek(lexer->input, lexer->pos, SEEK_SET);
    return buffer;
}
