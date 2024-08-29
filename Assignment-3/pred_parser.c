#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STACK_SIZE 100

typedef struct {
    int index;
    char elements[STACK_SIZE];
} StackStruct;

void stackPush(StackStruct *stack, char element) {
    if (stack->index < STACK_SIZE - 1) {
        stack->elements[++(stack->index)] = element;
    } else {
        fprintf(stderr, "Error: Stack is full\n");
    }
}

char stackPop(StackStruct *stack) {
    return (stack->index >= 0) ? stack->elements[(stack->index)--] : '\0';
}

char stackTop(StackStruct *stack) {
    return (stack->index >= 0) ? stack->elements[stack->index] : '\0';
}

void sanitizeInput(char* input) {
    int readIndex = 0, writeIndex = 0;
    int inIdentifier = 0;

    while (input[readIndex] != '\0') {
        if (isalnum(input[readIndex]) || input[readIndex] == '_') {
            if (!inIdentifier) {
                input[writeIndex++] = 'i';
                inIdentifier = 1;
            }
        } else {
            if (input[readIndex] != ' ') {
                input[writeIndex++] = input[readIndex];
            }
            inIdentifier = 0;
        }
        readIndex++;
    }

    input[writeIndex] = '\0';
}

int symbolToIndex(char symbol, const char* symbolSet) {
    char* pos = strchr(symbolSet, symbol);
    return (pos != NULL) ? (int)(pos - symbolSet) : -1;
}

void displayParseStep(int stepNum, StackStruct *stack, const char *input, const char *action) {
    printf("%-6d%-18s%-20s%-s\n", stepNum, stack->elements, input, action ? action : "");
}

void parseInput(char *input) {
    StackStruct stack = {-1, ""};
    const char *nonTerminals = "EDTSF";
    const char *terminals = "i+*()$";
    
    char *parseTable[5][6] = {
        {"TD", "",    "",    "TD", "",   ""  },
        {"",   "+TD", "",    "",   "^",  "^" },
        {"FS", "",    "",    "FS", "",   ""  },
        {"",   "^",   "*FS", "",   "^",  "^" },
        {"i",  "",    "",    "(E)", "",  ""  }
    };

    stackPush(&stack, '$');
    stackPush(&stack, 'E');

    char *currentInput = input;
    int stepCount = 1;

    printf("Step  Stack             Input               Action\n");
    printf("----- ----------------- ------------------- ------------\n");

    while (stack.index >= 0) {
        char topSymbol = stackTop(&stack);
        char inputSymbol = *currentInput;

        displayParseStep(stepCount++, &stack, currentInput, NULL);

        if (topSymbol == inputSymbol) {
            if (topSymbol == '$') {
                printf("\nInput Accepted\n");
                return;
            }
            stackPop(&stack);
            currentInput++;
        } else {
            int row = symbolToIndex(topSymbol, nonTerminals);
            int col = symbolToIndex(inputSymbol, terminals);

            if (row != -1 && col != -1) {
                char *production = parseTable[row][col];
                if (*production == '\0') {
                    fprintf(stderr, "Error: No production for %c -> %c\n", topSymbol, inputSymbol);
                    return;
                }

                stackPop(&stack);
                
                char actionMsg[20];
                snprintf(actionMsg, sizeof(actionMsg), "%c -> %s", topSymbol, production);
                displayParseStep(stepCount++, &stack, currentInput, actionMsg);

                for (int i = strlen(production) - 1; i >= 0; i--) {
                    if (production[i] != '^') {
                        stackPush(&stack, production[i]);
                    }
                }
            } else {
                fprintf(stderr, "Error: Invalid symbol %c\n", inputSymbol);
                return;
            }
        }
    }
}

int main() {
    char userInput[STACK_SIZE];
    
    printf("Symbol assumptions:\nE' -> D\nT' -> S\nid -> i\nε  -> ^\n");
    printf("Enter input string (end with $): ");
    
    if (fgets(userInput, sizeof(userInput), stdin) != NULL) {
        userInput[strcspn(userInput, "\n")] = 0;  // Remove newline if present
        sanitizeInput(userInput);
        parseInput(userInput);
    } else {
        fprintf(stderr, "Error reading input\n");
    }

    return 0;
}