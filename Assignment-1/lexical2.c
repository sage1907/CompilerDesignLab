#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ARITHMETIC_OP_TOKEN 256
#define LOGICAL_OP_TOKEN 257
#define BITWISE_OP_TOKEN 258
#define ASSIGNMENT_OP_TOKEN 259
#define RESERVED_WORD_TOKEN 260
#define IDENTIFIER_TOKEN 261
#define INT_CONST_TOKEN 262
#define FLOAT_CONST_TOKEN 263 
#define STRING_LITERAL_TOKEN 264
#define CHAR_CONST_TOKEN 265
#define L_PARENTHESIS_TOKEN 266
#define R_PARENTHESIS_TOKEN 267
#define L_CURLY_BRACE_TOKEN 268
#define R_CURLY_BRACE_TOKEN 269
#define L_SQUARE_BRACKET_TOKEN 270
#define R_SQUARE_BRACKET_TOKEN 271
#define SEMICOLON_TOKEN 272
#define COMMA_TOKEN 273
#define HEADER_FILE_TOKEN 274
#define MACRO_TOKEN 275
#define ADDRESS_OF_TOKEN 276
#define POINTER_TOKEN 277
#define COMMENT_TOKEN 278
#define EOF_TOKEN 279
#define UNKNOWN_TOKEN 280
#define RELATIONAL_OP_TOKEN 281
#define ERROR_TOKEN 500

#define MAX_TOKEN_LENGTH 100

FILE *yyin;
char yytext[MAX_TOKEN_LENGTH];
int yyleng = 0;
char current_char;
int current_line = 1;

const char *reserved_words[] = {
    "auto", "else", "long", "switch", "break", "enum", "register", "typedef", "case", "extern", "return", "union", "char", "float", "short", "unsigned", "const", "for", "signed", "void", "continue", "goto", "sizeof", "volatile", "default", "if", "static", "while", "do", "int", "struct", "_Packed", "double"
};

void fetch_char() {
    current_char = fgetc(yyin);
    if (current_char == '\n') {
        current_line++;
    }
}

void append_token_char() {
    if (yyleng < MAX_TOKEN_LENGTH - 1) {
        yytext[yyleng++] = current_char;
        yytext[yyleng] = '\0';
    }
}

void reset_token() {
    yyleng = 0;
    yytext[0] = '\0';
}

int is_reserved_word(char *word) {
    for (int i = 0; i < sizeof(reserved_words) / sizeof(char *); i++) {
        if (strcmp(word, reserved_words[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_num(char c) {
    return (c >= '0' && c <= '9');
}

int yylex() {
    reset_token();

    while (isspace(current_char)) {
        fetch_char();
    }

    if (current_char == EOF) {
        return EOF_TOKEN;
    }

    if (isalpha(current_char) || current_char == '_') {
        while (isalnum(current_char) || current_char == '_') {
            append_token_char();
            fetch_char();
        }
        if (is_reserved_word(yytext)) {
            return RESERVED_WORD_TOKEN;
        }
        return IDENTIFIER_TOKEN;
    }

    if (is_num(current_char)) {
        int is_float = 0;
        while (is_num(current_char)) {
            append_token_char();
            fetch_char();
        }
        if (current_char == '.') {
            is_float = 1;
            append_token_char();
            fetch_char();
            while (is_num(current_char)) {
                append_token_char();
                fetch_char();
            }
        }
        if (isalnum(current_char)) {
            return UNKNOWN_TOKEN;
        }
        return is_float ? FLOAT_CONST_TOKEN : INT_CONST_TOKEN;
    }

    if (current_char == '\"') {
        append_token_char();
        fetch_char();
        while (current_char != '\"' && current_char != EOF && current_char != ';') {
            append_token_char();
            fetch_char();
        }
        if (current_char == '\"') {
            append_token_char();
            fetch_char();
            return STRING_LITERAL_TOKEN;
        }
        fprintf(stderr, "Found unterminated string literal at line %d\n", current_line);
        return ERROR_TOKEN;
    }

    if (current_char == '\'') {
        append_token_char();
        fetch_char();
        if (current_char != EOF) {
            append_token_char();
            fetch_char();
            if (current_char == '\'') {
                append_token_char();
                fetch_char();
                return CHAR_CONST_TOKEN;
            }
        }
        fprintf(stderr, "Invalid character constant found at line %d\n", current_line);
        return ERROR_TOKEN;
    }

    if (current_char == '#') {
        append_token_char();
        fetch_char();
        while (isspace(current_char)) {
            fetch_char();
        }
        
        while (!isspace(current_char) && current_char != EOF) {
            append_token_char();
            fetch_char();
        }
        return MACRO_TOKEN;
    }

    if (current_char == '&') {
        append_token_char();
        fetch_char();
        if (current_char == '&') {
            append_token_char();
            fetch_char();
            return LOGICAL_OP_TOKEN;
        }
        return ADDRESS_OF_TOKEN;
    }

    if (current_char == '*') {
        append_token_char();
        fetch_char();
        return ARITHMETIC_OP_TOKEN;
    }

    if (current_char == '/') {
        append_token_char();
        fetch_char();
        if (current_char == '/') {
            while (current_char != '\n' && current_char != EOF) {
                append_token_char();
                fetch_char();
            }
            return COMMENT_TOKEN;
        } else if (current_char == '*') {
            append_token_char();
            fetch_char();
            while (1) {
                if (current_char == '*') {
                    append_token_char();
                    if ((current_char = fgetc(yyin)) == '/')
                    {
                        append_token_char();
                        fetch_char();
                        break;
                    }
                }
                if (current_char == EOF) {
                    fprintf(stderr, "Comment is not terminated at line %d\n", current_line);
                    return ERROR_TOKEN;
                }
                if (current_char == '\n')
                    current_char = ' ';
                append_token_char();
                fetch_char();
            }
            return COMMENT_TOKEN;
        } else {
            return ARITHMETIC_OP_TOKEN;
        }
    }

    switch (current_char) {
        case '+': case '-': case '%':
            append_token_char();
            fetch_char();
            return ARITHMETIC_OP_TOKEN;
        case '=':
            append_token_char();
            fetch_char();
            if (current_char == '=') {
                append_token_char();
                fetch_char();
                return RELATIONAL_OP_TOKEN;
            }
            return ASSIGNMENT_OP_TOKEN;
        case '<':
            append_token_char();
            fetch_char();
            if (current_char == '=') {
                append_token_char();
                fetch_char();
                return RELATIONAL_OP_TOKEN;
            }
            else if (current_char != ' ' && !is_num(current_char)) {
                while (!isspace(current_char) && current_char != EOF) {
                    append_token_char();
                    fetch_char();
                }
                return HEADER_FILE_TOKEN;
            }
            return RELATIONAL_OP_TOKEN;
        case '>':
            append_token_char();
            fetch_char();
            if (current_char == '=') {
                append_token_char();
                fetch_char();
                return RELATIONAL_OP_TOKEN;
            }
            return RELATIONAL_OP_TOKEN;
        case '|':
            append_token_char();
            fetch_char();
            if (current_char == '|') {
                append_token_char();
                fetch_char();
                return LOGICAL_OP_TOKEN;
            }
            return BITWISE_OP_TOKEN;
        case '^': case '~':
            append_token_char();
            fetch_char();
            return BITWISE_OP_TOKEN;
        case '(':
            append_token_char();
            fetch_char();
            return L_PARENTHESIS_TOKEN;
        case ')':
            append_token_char();
            fetch_char();
            return R_PARENTHESIS_TOKEN;
        case '{':
            append_token_char();
            fetch_char();
            return L_CURLY_BRACE_TOKEN;
        case '}':
            append_token_char();
            fetch_char();
            return R_CURLY_BRACE_TOKEN;
        case '[':
            append_token_char();
            fetch_char();
            return L_SQUARE_BRACKET_TOKEN;
        case ']':
            append_token_char();
            fetch_char();
            return R_SQUARE_BRACKET_TOKEN;
        case ';':
            append_token_char();
            fetch_char();
            return SEMICOLON_TOKEN;
        case ',':
            append_token_char();
            fetch_char();
            return COMMA_TOKEN;
        default:
            fprintf(stderr, "Unknown character '%c' encountered at line %d\n", current_char, current_line);
            append_token_char();
            fetch_char();
            return UNKNOWN_TOKEN;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        perror("Error opening file");
        return 1;
    }

    printf("%-50s %-10s\n", "Token", "Type");
    printf("%-50s %-10s\n", "-----", "----");

    fetch_char();
    while (!feof(yyin)) {
        int token = yylex();
        if (token == EOF_TOKEN) break;
        printf("%-50s %-10d\n", yytext, token);
    }

    fclose(yyin);
    return 0;
}
