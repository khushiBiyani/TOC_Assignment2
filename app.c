#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX (1000000)

/*
       _       _           _                 _       _     _
      | |     | |         | |               (_)     | |   | |
  __ _| | ___ | |__   __ _| |_   ____ _ _ __ _  __ _| |__ | | ___  ___
 / _` | |/ _ \| '_ \ / _` | \ \ / / _` | '__| |/ _` | '_ \| |/ _ \/ __|
| (_| | | (_) | |_) | (_| | |\ V / (_| | |  | | (_| | |_) | |  __/\__ \
 \__, |_|\___/|_.__/ \__,_|_| \_/ \__,_|_|  |_|\__,_|_.__/|_|\___||___/
  __/ |
 |___/

*/

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
    TOKEN_OPERATOR_DOUBLE_EQUAL,
    TOKEN_LPARAN,
    TOKEN_RPARAN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_VARIABLE,
    TOKEN_INTEGER_LITERAL,
};

char programCode[MAX];
int programCodeSize;

/*
      _                  _                   _
     | |                (_)                 | |
  ___| | ___  __ _ _ __  _ _ __  _ __  _   _| |_
 / __| |/ _ \/ _` | '_ \| | '_ \| '_ \| | | | __|
| (__| |  __/ (_| | | | | | | | | |_) | |_| | |_
 \___|_|\___|\__,_|_| |_|_|_| |_| .__/ \__,_|\__|
                                | |
                                |_|

*/

void readCode(char *filename) {
    FILE *in_file = fopen(filename, "r");

    char line[MAX];
    char prevChar = '\0';
    char currentChar = '\0';
    while (fgets(line, MAX, in_file)) {
        for (int i = 0; i < strlen(line); ++i) {
            currentChar = line[i];
            if (currentChar == 10 || currentChar == 13) continue;
            if (currentChar == ' ' && prevChar == ' ') continue;
            programCode[programCodeSize++] = currentChar;
            prevChar = currentChar;
        }
    }
}

/*
 _
| |
| | _____  _____ _ __
| |/ _ \ \/ / _ \ '__|
| |  __/>  <  __/ |
|_|\___/_/\_\___|_|
*/

bool isSpecialChar(char c) {
    return c == ';' || c == ' ' || c == ',' || c == '+' || c == '-' || c == '*' || c == '/' || c == '>' || c == '=' || c == '(' || c == ')' || c == '{' || c == '}';
}

struct nextToken_t {
    char data[MAX];
    int size;
    enum TOKEN_TYPE type;
};

struct nextToken_t nextToken;

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

void classifyToken() {
    if (nextToken.size == 1) {
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
    } else if (nextToken.size == 2) {
        if (!strcmp(nextToken.data, "==")) {
            nextToken.type = TOKEN_OPERATOR_DOUBLE_EQUAL;
        }
    } else if (nextToken.size == 3) {
        if (!strcmp(nextToken.data, "int")) {
            nextToken.type = TOKEN_INT;
        } else if (!strcmp(nextToken.data, "for")) {
            nextToken.type = TOKEN_FOR;
        }
    } else if (nextToken.size == 4) {
        if (!strcmp(nextToken.data, "read")) {
            nextToken.type = TOKEN_READ;
        }
    } else if (nextToken.size == 5) {
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
}

int ptr;

bool canReadToken() {
    return ptr < programCodeSize;
}

void getNextToken(bool ignoreWhitespace, bool peek) {
    int prevPtr = ptr;
    int tokenSize = 0;
    nextToken.type = INVALID_TOKEN;

    while (canReadToken()) {
        char currentChar = programCode[ptr];
        if (isSpecialChar(currentChar)) {
            if (tokenSize > 0) {
                break;
            }
            if (ptr + 1 < programCodeSize) {
                char nextChar = programCode[ptr + 1];
                if ((currentChar == '=') && (nextChar == '=')) {
                    nextToken.data[tokenSize++] = currentChar;
                    nextToken.data[tokenSize++] = nextChar;
                    ptr += 2;
                    break;
                }
            }
            nextToken.data[tokenSize++] = currentChar;
            ++ptr;
            break;
        } else {
            nextToken.data[tokenSize++] = currentChar;
            ++ptr;
        }
    }

    nextToken.data[tokenSize] = '\0';
    nextToken.size = tokenSize;

    classifyToken();

    if (ignoreWhitespace && nextToken.type == TOKEN_SPACE) getNextToken(false, peek);

    if (peek) {
        ptr = prevPtr;
    }
}

bool readToken(bool ignoreWhitespace, bool peek) {
    if (canReadToken()) {
        getNextToken(ignoreWhitespace, peek);
        return true;
    }
    return false;
}

/*
                 _                 _               _
                | |               | |             | |
 ___ _   _ _ __ | |_ __ ___  _____| |__   ___  ___| | _____ _ __
/ __| | | | '_ \| __/ _` \ \/ / __| '_ \ / _ \/ __| |/ / _ \ '__|
\__ \ |_| | | | | || (_| |>  < (__| | | |  __/ (__|   <  __/ |
|___/\__, |_| |_|\__\__,_/_/\_\___|_| |_|\___|\___|_|\_\___|_|
      __/ |
     |___/
*/

bool parseDeclaration(){
    if (readToken(true, false) && nextToken.type == TOKEN_INT){
        if (readToken(false, false) && nextToken.type == TOKEN_SPACE){
            while(true){
                if (readToken(true, false) && nextToken.type == TOKEN_VARIABLE){
                    if(readToken(true, false)){
                        if(nextToken.type == TOKEN_COMMA)
                            continue;
                        else if(nextToken.type == TOKEN_SEMICOLON)
                            break;
                        else
                            return false;
                    }
                }
                else
                    return false;
            }
            return true;
        }
    }
    return false;
}

bool parseRead() {
    if (readToken(true, false) && nextToken.type == TOKEN_READ) {
        if (readToken(false, false) && nextToken.type == TOKEN_SPACE) {
            if (readToken(true, false) && nextToken.type == TOKEN_VARIABLE) {
                if (readToken(true, false) && nextToken.type == TOKEN_SEMICOLON) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool parseWrite() {
    if (readToken(true, false) && nextToken.type == TOKEN_WRITE) {
        if (readToken(false, false) && nextToken.type == TOKEN_SPACE) {
            if (readToken(true, false) && (nextToken.type == TOKEN_VARIABLE || nextToken.type == TOKEN_INTEGER_LITERAL)) {
                if (readToken(true, false) && nextToken.type == TOKEN_SEMICOLON) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool parseForLoop() {
    // TODO: Complete with grammer written by Ankesh
    return false;
}

bool parseExpression() {
    // TODO: This is some tough function
    return false;
}

bool parseAssignment() {
    if (readToken(true, false) && nextToken.type == TOKEN_VARIABLE) {
        if (readToken(true, false) && nextToken.type == TOKEN_OPERATOR_EQUAL) {
            if (parseExpression()) {
                if (readToken(true, false) && nextToken.type == TOKEN_SEMICOLON) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool parseStatement() {
    if (readToken(true, true)) {
        switch (nextToken.type) {
            case TOKEN_READ: {
                return parseRead();
            }
            case TOKEN_WRITE: {
                return parseWrite();
            }
            case TOKEN_FOR: {
                return parseForLoop();
            }
            case TOKEN_VARIABLE: {
                return parseAssignment();
            }
            default: {
                return false;
            }
        }
    } else {
        return false;
    }
}

bool parseFirstStatement() {
    if (readToken(true, true)) {
        switch (nextToken.type) {
            case TOKEN_INT: {
                return parseDeclaration();
            }
            default: {
                return parseStatement();
            }
        }
    } else {
        return false;
    }
}

bool parseProgram() {
    if (!parseFirstStatement()) return false;
    while (canReadToken()) {
        if (!parseStatement()) return false;
    }
    return true;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ./simulator <input.txt>\n");
        exit(1);
    }

    readCode(argv[1]);

    printf("Grammer is: %s\n", parseProgram() ? "Correct" : "Incorrect");
}
