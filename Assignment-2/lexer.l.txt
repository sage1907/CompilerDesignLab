%{
#include <stdio.h>
#include <string.h>
#include "tokens.h"

extern int yyleng;
extern char *yytext;

int current_line = 0;
void yyerror(const char *msg);

// List of reserved words
const char *reserved_words[] = {
    "int", "float", "char", "for", "do", "while", "if", "else", "return", "void", "break", "continue",
    "switch", "case", "default", "typedef", "struct", "union", "enum", "goto", "volatile", "static",
    "extern", "register", "const", "signed", "unsigned", "long", "short", "double", "sizeof", "auto"
};

int is_reserved_word(const char *word) {
    for (int i = 0; i < sizeof(reserved_words) / sizeof(char *); i++) {
        if (strcmp(word, reserved_words[i]) == 0) {
            return 1;
        }
    }
    return 0;
}
%}


%%

"<"[a-zA-Z0-9_]+\.h">"  { return HEADER_FILE_TOKEN; }
"+"                 { return ARITHMETIC_OP_TOKEN; }
"-"                 { return ARITHMETIC_OP_TOKEN; }
"*"                 { return ARITHMETIC_OP_TOKEN; }
"/"                 { return ARITHMETIC_OP_TOKEN; }
"%"                 { return ARITHMETIC_OP_TOKEN; }

"=="                { return RELATIONAL_OP_TOKEN; }
"!="                { return RELATIONAL_OP_TOKEN; }
"<="                { return RELATIONAL_OP_TOKEN; }
">="                { return RELATIONAL_OP_TOKEN; }
"<"                 { return RELATIONAL_OP_TOKEN; }
">"                 { return RELATIONAL_OP_TOKEN; }

"&&"                { return LOGICAL_OP_TOKEN; }
"||"                { return LOGICAL_OP_TOKEN; }
"!"                 { return LOGICAL_OP_TOKEN; }

"&"                 { return BITWISE_OP_TOKEN; }
"|"                 { return BITWISE_OP_TOKEN; }
"^"                 { return BITWISE_OP_TOKEN; }
"~"                 { return BITWISE_OP_TOKEN; }

"="                 { return ASSIGNMENT_OP_TOKEN; }

\"[^\"]*\"          { return STRING_LITERAL_TOKEN; }
\'[^\']\'           { return CHAR_CONST_TOKEN; }

[a-zA-Z_][a-zA-Z0-9_]* {
    if (is_reserved_word(yytext)) {
        return RESERVED_WORD_TOKEN;
    }
    return IDENTIFIER_TOKEN;
}

[0-9]+\.[0-9]*([eE][+-]?[0-9]+)?  { return FLOAT_CONST_TOKEN; }
\.[0-9]+([eE][+-]?[0-9]+)?        { return FLOAT_CONST_TOKEN; }
[0-9]+[eE][+-]?[0-9]+             { return FLOAT_CONST_TOKEN; }

[0-9]+              		 { return INT_CONST_TOKEN; }

"("                		 { return L_PARENTHESIS_TOKEN; }
")"                		 { return R_PARENTHESIS_TOKEN; }

"{"                		 { return L_CURLY_BRACE_TOKEN; }
"}"                		 { return R_CURLY_BRACE_TOKEN; }

"["                		 { return L_SQUARE_BRACKET_TOKEN; }
"]"                		 { return R_SQUARE_BRACKET_TOKEN; }

";"                		 { return SEMICOLON_TOKEN; }
","                		 { return COMMA_TOKEN; }

"#include"         		 { return MACRO_TOKEN; }
"#define"          		 { return MACRO_TOKEN; }

"//".*             		 { return COMMENT_TOKEN; }
"/*"([^*]*\*+)*"*/"		 { return COMMENT_TOKEN; }

[ \t]            		 { /* Ignore whitespace */ }
\n 				         { current_line++; }
.                  		 { yyerror("Unknown character encountered"); return UNKNOWN_TOKEN; }

%%

void yyerror(const char *msg) {
    fprintf(stderr, "Error: %s at line %d: %s\n", msg, current_line, yytext);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    yyin = file;
    int token;
    while ((token = yylex()) != 0) {
        printf("Token: %s\tType: %d\n", yytext, token);
    }

    fclose(file);
    return 0;
}