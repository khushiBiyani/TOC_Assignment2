#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX (1000000)

enum TOKEN_TYPE {
    INVALID_TOKEN,
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_SPACE,
    TOKEN_INT,
    TOKEN_FOR,
    TOKEN_READ,
    TOKEN_WRITE,
    TOKEN_OPERATOR_ADD,
    TOKEN_OPERATOR_SUB,
    TOKEN_OPERATOR_MUL,
    TOKEN_OPERATOR_DIV,
    TOKEN_OPERATOR_GREATER_THAN,
    TOKEN_OPERATOR_EQUAL,
    TOKEN_LPARAN,
    TOKEN_RPARAN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_VARIABLE,
    TOKEN_INTEGER_LITERAL,
};

char programCode[MAX];
int programCodeSize;

void readCode(char *filename) {
    FILE *in_file = fopen(filename, "r");

    char line[MAX];
    char prevChar = 0;
    char currentChar = 0;
    while (fgets(line, MAX, in_file)) {
        for (int i = 0; i < strlen(line); ++i) {
            currentChar = line[i];
            if (currentChar == '\n') continue;
            if (currentChar == ' ' && prevChar == ' ') continue;
            programCode[programCodeSize++] = currentChar;
            prevChar = currentChar;
        }
    }
}

struct nextToken_t {
    char data[MAX];
    int size;
    enum TOKEN_TYPE type;
};

struct nextToken_t nextToken;

bool isSpecialChar(char c) {
    return c == ';' || c == ' ' || c == ',' || c == '+' || c == '-' || c == '*' || c == '/' || c == '>' || c == '=' || c == '(' || c == ')' || c == '{' || c == '}';
}

bool checkNextTokenIsVariable() {
    for (int i = 0; i < nextToken.size; ++i) {
        if (nextToken.data[i] < 'a' || nextToken.data[i] > 'z') {
            return false;
        }
    }
    nextToken.type = TOKEN_VARIABLE;
    return true;
}

bool checkNextTokenIsIntegerLiteral() {
    for (int i = 0; i < nextToken.size; ++i) {
        if (nextToken.data[i] < '0' || nextToken.data[i] > '9') {
            return false;
        }
    }
    nextToken.type = TOKEN_INTEGER_LITERAL;
    return true;
}

bool getNextToken(bool ignoreWhitespace) {
    static int ptr = 0;
    if (ptr >= programCodeSize) return false;

    int tokenSize = 0;
    nextToken.type = INVALID_TOKEN;
    while (ptr < programCodeSize) {
        char currentChar = programCode[ptr];
        if (isSpecialChar(currentChar)) {
            if (!tokenSize) {
                nextToken.data[tokenSize++] = currentChar;
                ++ptr;
            }
            break;
        } else {
            nextToken.data[tokenSize++] = currentChar;
            ++ptr;
        }
    }

    if (tokenSize == 1) {
        switch (nextToken.data[0]) {
            case ';': {
                nextToken.type = TOKEN_SEMICOLON;
                break;
            }
            case ',': {
                nextToken.type = TOKEN_COMMA;
                break;
            }
            case ' ': {
                nextToken.type = TOKEN_SPACE;
                break;
            }
            case '+': {
                nextToken.type = TOKEN_OPERATOR_ADD;
                break;
            }
            case '-': {
                nextToken.type = TOKEN_OPERATOR_SUB;
                break;
            }
            case '*': {
                nextToken.type = TOKEN_OPERATOR_MUL;
                break;
            }
            case '/': {
                nextToken.type = TOKEN_OPERATOR_DIV;
                break;
            }
            case '>': {
                nextToken.type = TOKEN_OPERATOR_GREATER_THAN;
                break;
            }
            case '=': {
                nextToken.type = TOKEN_OPERATOR_EQUAL;
                break;
            }
            case '(': {
                nextToken.type = TOKEN_LPARAN;
                break;
            }
            case ')': {
                nextToken.type = TOKEN_RPARAN;
                break;
            }
            case '{': {
                nextToken.type = TOKEN_LBRACE;
                break;
            }
            case '}': {
                nextToken.type = TOKEN_RBRACE;
                break;
            }
        }
    }

    if (tokenSize == 3) {
        if (!strcmp(nextToken.data, "int")) {
            nextToken.type = TOKEN_INT;
        } else if (!strcmp(nextToken.data, "for")) {
            nextToken.type = TOKEN_FOR;
        }
    }

    if (tokenSize == 4) {
        if (!strcmp(nextToken.data, "read")) {
            nextToken.type = TOKEN_READ;
        }
    }

    if (tokenSize == 5) {
        if (!strcmp(nextToken.data, "write")) {
            nextToken.type = TOKEN_WRITE;
        }
    }

    if (nextToken.type == INVALID_TOKEN) {
        if (!checkNextTokenIsVariable() && !checkNextTokenIsIntegerLiteral()) {
            printf("Invalid token: %s\n", nextToken.data);
            exit(1);
        }
    }

    nextToken.data[tokenSize] = 0;
    nextToken.size = tokenSize;

    if (ignoreWhitespace && tokenSize == 1 && nextToken.data[0] == ' ')
        return getNextToken(false);
    else
        return true;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ./simulator <input.txt>\n");
        exit(1);
    }

    readCode(argv[1]);

    while (getNextToken(true)) {
        printf("%s {%d}\n", nextToken.data, nextToken.type);
    }
}
