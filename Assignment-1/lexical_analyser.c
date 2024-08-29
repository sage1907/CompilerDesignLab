//Anish Banerjee - 2021CSB001

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100

#define ARITHMETIC_OP_TOK 256
#define RELATIONAL_OP_TOK 257
#define LOGICAL_OP_TOK 258
#define BITWISE_OP_TOK 259
#define ASSIGNMENT_OP_TOK 260
#define RESERVED_WORD_TOK 261
#define IDENTIFIER_TOK 262
#define INT_CONST_TOK 263
#define STRING_LITERAL_TOK 264
#define CHAR_CONST_TOK 265
#define L_PARENTHESIS_TOK 266
#define R_PARENTHESIS_TOK 267
#define L_CURLY_BRACE_TOK 268
#define R_CURLY_BRACE_TOK 269
#define L_SQUARE_BRACKET_TOK 270
#define R_SQUARE_BRACKET_TOK 271
#define SEMICOLON_TOK 272
#define COMMA_TOK 273
#define HEADER_FILE_TOK 274
#define MACRO_TOK 275
#define ADDRESS_OF_TOK 276
#define POINTER_TOK 277
#define FUNCTION_TOK 278
#define COMMENT_TOK 279
#define EOF_TOK 280
#define UNKNOWN_TOK 281
#define ERROR_TOK 500

FILE *yyin;
char yytext[MAX_TOKEN_LENGTH];
int yyleng = 0;
char current_char;
int line_number = 1;

const char *reserved_words[] = {
    "auto", "else", "long", "switch", "break", "enum", "register", "typedef", "case", "extern", "return", "union", "char", "float", "short", "unsigned", "const", "for", "signed", "void", "continue", "goto", "sizeof", "volatile", "default", "if", "static", "while", "do", "int", "struct", "_Packed", "double"
};

const char *functions[] = {
    "fopen", "freopen", "fclose", "fflush", "fseek", "ftell", "rewind", "fprintf", "fscanf", "printf", "scanf", "vfprintf", "vfscanf", "vprintf", "vsprintf", "vsnprintf", "sprintf", "snprintf", "fputc", "fgetc", "fgets", "fputs", "getc", "putc", "getchar", "putchar", "ungetc", "feof", "ferror", "clearerr", "perror", "setvbuf", "getchar_unlocked", "putchar_unlocked", "getopt", "abort", "atexit", "atof", "atoi", "atol", "strtod", "strtof", "strtold", "strtol", "strtoll", "strtoul", "strtoull", "malloc", "calloc", "realloc", "free", "exit", "getenv", "system", "bsearch", "qsort", "abs", "labs", "llabs", "div", "ldiv", "lldiv", "mblen", "mbstowcs", "wctomb", "mbtowc", "wcstombs", "wcstoul", "wcstoll", "wcstoll", "rand", "srand", "rand_r", "memcpy", "memmove", "memset", "memcmp", "strlen", "strcpy", "strncpy", "strcat", "strncat", "strcmp", "strncmp", "strchr", "strrchr", "strstr", "strtok", "strspn", "strcspn", "strpbrk", "strtol", "strtoul", "strtod", "strtof", "strtold", "memchr", "memccpy", "mempcpy", "isalnum", "isalpha", "isascii", "isdigit", "islower", "isupper", "isxdigit", "isgraph", "isprint", "iscntrl", "isspace", "tolower", "toupper", "abs", "fabs", "fabsf", "fabs", "ceil", "floor", "fmod", "modf", "sqrt", "pow", "exp", "log", "log10", "sin", "cos", "tan", "asin", "acos", "atan", "atan2", "sinh", "cosh", "tanh", "erf", "erfc", "tgamma", "lgamma", "frexp", "ldexp", "nearbyint", "rint", "lrint", "llrint", "fma", "fmin", "fmax", "fmod", "remainder", "remquo", "clock", "time", "difftime", "mktime", "strftime", "asctime", "ctime", "gmtime", "localtime", "tzset", "getdate", "errno", "perror", "strerror", "signal", "raise", "sigaction", "sigemptyset", "sigfillset", "sigaddset", "sigdelset", "sigismember", "sighold", "sigpause", "sigrelse", "sigwait", "sigwaitinfo", "sigtimedwait", "sigpending", "sigprocmask", "sigset", "kill", "va_start", "va_arg", "va_end", "va_copy", "assert", "setlocale", "localeconv", "duplocale", "freelocale", "newlocale", "uselocale", "offsetof", "size_t", "ptrdiff_t", "wchar_t", "bool", "true", "false", "creal", "cimag", "cexp", "clog", "creal", "cimag", "conj", "cabs", "carg", "cexp", "clog", "cpow"
};

void get_next_char() {
    current_char = fgetc(yyin);
    if (current_char == '\n') {
        line_number++;
    }
}

void append_char() {
    if (yyleng < MAX_TOKEN_LENGTH - 1) {
        yytext[yyleng++] = current_char;
        yytext[yyleng] = '\0';
    }
}

void reset_yytext() {
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

int is_function(char *word) {
    for (int i = 0; i < sizeof(functions) / sizeof(char *); i++) {
        if (strcmp(word, functions[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

int yylex() {
    reset_yytext();

    while (isspace(current_char)) {
        get_next_char();
    }

    if (current_char == EOF) {
        return EOF_TOK;
    }

    if (isalpha(current_char) || current_char == '_') {
        while (isalnum(current_char) || current_char == '_') {
            append_char();
            get_next_char();
        }
        if (is_function(yytext)) {
            return FUNCTION_TOK;
        }
        if (is_reserved_word(yytext)) {
            return RESERVED_WORD_TOK;
        }
        return IDENTIFIER_TOK;
    }

    if (isdigit(current_char)) {
        while (isdigit(current_char)) {
            append_char();
            get_next_char();
        }
        if (isalnum(current_char))
            return UNKNOWN_TOK;
        return INT_CONST_TOK;
    }

    if (current_char == '\"') {
        append_char();
        get_next_char();
        while (current_char != '\"' && current_char != EOF && current_char != ';') {
            append_char();
            get_next_char();
        }
        if (current_char == '\"') {
            append_char();
            get_next_char();
            return STRING_LITERAL_TOK;
        }
        fprintf(stderr, "Error: Unterminated string literal at line %d\n", line_number);
        return ERROR_TOK;
    }

    if (current_char == '\'') {
        append_char();
        get_next_char();
        if (current_char != EOF) {
            append_char();
            get_next_char();
            if (current_char == '\'') {
                append_char();
                get_next_char();
                return CHAR_CONST_TOK;
            }
        }
        fprintf(stderr, "Error: Invalid character constant at line %d\n", line_number);
        return ERROR_TOK;
    }

    if (current_char == '#') {
        append_char();
        get_next_char();
        while (isspace(current_char)) {
            get_next_char();
        }
        
        while (!isspace(current_char) && current_char != EOF) {
            append_char();
            get_next_char();
        }
        return MACRO_TOK;
    }

    if (current_char == '&') {
        append_char();
        get_next_char();
        if (current_char == '&') {
            append_char();
            get_next_char();
            return LOGICAL_OP_TOK;
        }
        return ADDRESS_OF_TOK;
    }

    if (current_char == '*') {
        append_char();
        get_next_char();
        return ARITHMETIC_OP_TOK;
    }

    if (current_char == '/') {
        append_char();
        get_next_char();
        if (current_char == '/') {
            while (current_char != '\n' && current_char != EOF) {
                append_char();
                get_next_char();
            }
            return COMMENT_TOK;
        } else if (current_char == '*') {
            append_char();
            get_next_char();
            while (1) {
                if (current_char == '*') {
                    append_char();
                    if ((current_char = fgetc(yyin)) == '/')
                    {
                        append_char();
                        get_next_char();
                        break;
                    }
                }
                if (current_char == EOF) {
                    fprintf(stderr, "Error: Unterminated comment at line %d\n", line_number);
                    return ERROR_TOK;
                }
                if (current_char == '\n')
                    current_char=' ';
                append_char();
                get_next_char();
            }
            return COMMENT_TOK;
        } else {
            return ARITHMETIC_OP_TOK;
        }
    }

    switch (current_char) {
        case '+': case '-': case '%':
            append_char();
            get_next_char();
            return ARITHMETIC_OP_TOK;
        case '=':
            append_char();
            get_next_char();
            if (current_char == '=') {
                append_char();
                get_next_char();
                return RELATIONAL_OP_TOK;
            }
            return ASSIGNMENT_OP_TOK;
        case '<':
            append_char();
            get_next_char();
            if (current_char == '=') {
                append_char();
                get_next_char();
                return RELATIONAL_OP_TOK;
            }
            else if (current_char != ' ' && !is_digit(current_char))
            {
                while (!isspace(current_char) && current_char != EOF) {
                    append_char();
                    get_next_char();
                }
                return HEADER_FILE_TOK;
            }
            return RELATIONAL_OP_TOK;
        
        case '>':
            append_char();
            get_next_char();
            if (current_char == '=') {
                append_char();
                get_next_char();
                return RELATIONAL_OP_TOK;
            }
            return RELATIONAL_OP_TOK;
        case '|':
            append_char();
            get_next_char();
            if (current_char == '|') {
                append_char();
                get_next_char();
                return LOGICAL_OP_TOK;
            }
            return BITWISE_OP_TOK;
        case '^': case '~':
            append_char();
            get_next_char();
            return BITWISE_OP_TOK;
        case '(':
            append_char();
            get_next_char();
            return L_PARENTHESIS_TOK;
        case ')':
            append_char();
            get_next_char();
            return R_PARENTHESIS_TOK;
        case '{':
            append_char();
            get_next_char();
            return L_CURLY_BRACE_TOK;
        case '}':
            append_char();
            get_next_char();
            return R_CURLY_BRACE_TOK;
        case '[':
            append_char();
            get_next_char();
            return L_SQUARE_BRACKET_TOK;
        case ']':
            append_char();
            get_next_char();
            return R_SQUARE_BRACKET_TOK;
        case ';':
            append_char();
            get_next_char();
            return SEMICOLON_TOK;
        case ',':
            append_char();
            get_next_char();
            return COMMA_TOK;
        default:
            fprintf(stderr, "Error: Unknown character '%c' at line %d\n", current_char, line_number);
            append_char();
            get_next_char();
            return UNKNOWN_TOK;
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

    get_next_char();
    while (!feof(yyin)) {
        int token = yylex();
        if (token == EOF_TOK) break;
        printf("%-50s %-10d\n", yytext, token);
    }

    fclose(yyin);
    return 0;
}
