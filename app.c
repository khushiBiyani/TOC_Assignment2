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
            if (currentChar == 10 || currentChar == 13 || (currentChar == ' ' && prevChar == ' ')) continue;
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
     _                            _   _             _        _
    | |                          | | (_)           | |      | |
  __| | ___  ___ _ __ _   _ _ __ | |_ _ _ __   __ _| |_ ___ | | _____ _ __
 / _` |/ _ \/ __| '__| | | | '_ \| __| | '_ \ / _` | __/ _ \| |/ / _ \ '_ \
| (_| |  __/ (__| |  | |_| | |_) | |_| | | | | (_| | || (_) |   <  __/ | | |
 \__,_|\___|\___|_|   \__, | .__/ \__|_|_| |_|\__, |\__\___/|_|\_\___|_| |_|
                       __/ | |                 __/ |
                      |___/|_|                |___/
*/

void decryptToken() {
    int tokenType = nextToken.type;
    switch (tokenType) {
        case INVALID_TOKEN: {
            printf("Invalid token");
            break;
        }
        case TOKEN_SEMICOLON: {
            printf(";");
            break;
        }
        case TOKEN_COMMA: {
            printf(",");
            break;
        }
        case TOKEN_SPACE: {
            printf(" ");
            break;
        }
        case TOKEN_INT: {
            printf("int");
            break;
        }
        case TOKEN_FOR: {
            printf("for");
            break;
        }
        case TOKEN_READ: {
            printf("read");
            break;
        }
        case TOKEN_WRITE: {
            printf("write");
            break;
        }
        case TOKEN_OPERATOR_ADD: {
            printf("+");
            break;
        }
        case TOKEN_OPERATOR_SUB: {
            printf("-");
            break;
        }
        case TOKEN_OPERATOR_MUL: {
            printf("*");
            break;
        }
        case TOKEN_OPERATOR_DIV: {
            printf("/");
            break;
        }
        case TOKEN_OPERATOR_GREATER_THAN: {
            printf(">");
            break;
        }
        case TOKEN_OPERATOR_EQUAL: {
            printf("=");
            break;
        }
        case TOKEN_OPERATOR_DOUBLE_EQUAL: {
            printf("==");
            break;
        }
        case TOKEN_LPARAN: {
            printf("(");
            break;
        }
        case TOKEN_RPARAN: {
            printf(")");
            break;
        }
        case TOKEN_LBRACE: {
            printf("{");
            break;
        }
        case TOKEN_RBRACE: {
            printf("}");
            break;
        }
        case TOKEN_VARIABLE: {
            printf("Variable");
            break;
        }
        case TOKEN_INTEGER_LITERAL: {
            printf("Constant");
            break;
        }
    }
    puts("");
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

bool parseDeclaration() {
    if (readToken(true, false) && nextToken.type == TOKEN_INT) {
        if (readToken(false, false) && nextToken.type == TOKEN_SPACE) {
            while (true) {
                if (readToken(true, false) && nextToken.type == TOKEN_VARIABLE) {
                    if (readToken(true, false)) {
                        if (nextToken.type == TOKEN_COMMA)
                            continue;
                        else if (nextToken.type == TOKEN_SEMICOLON)
                            break;
                        else
                            return false;
                    }
                } else
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

bool parseExpressionWithoutSemiColon();

bool parseFactor() {
    if (readToken(true, false)) {
        if (nextToken.type == TOKEN_VARIABLE || nextToken.type == TOKEN_INTEGER_LITERAL) {
            return true;
        }
        if (nextToken.type == TOKEN_LPARAN) {
            if (parseExpressionWithoutSemiColon()) {
                if (readToken(true, false) && nextToken.type == TOKEN_RPARAN) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool term() {
    if (parseFactor()) {
        if (readToken(true, true)) {
            if (nextToken.type == TOKEN_SEMICOLON || nextToken.type == TOKEN_RPARAN) {
                return true;
            }
            if (nextToken.type == TOKEN_OPERATOR_DIV || nextToken.type == TOKEN_OPERATOR_MUL || nextToken.type == TOKEN_OPERATOR_ADD || nextToken.type == TOKEN_OPERATOR_SUB || nextToken.type == TOKEN_OPERATOR_GREATER_THAN || nextToken.type == TOKEN_OPERATOR_DOUBLE_EQUAL) {
                readToken(true, false);
                return term();
            }
        }
    }
    return false;
}

bool value() {
    if (term()) {
        if (readToken(true, true)) {
            if (nextToken.type == TOKEN_SEMICOLON || nextToken.type == TOKEN_RPARAN) {
                return true;
            }
            if (nextToken.type == TOKEN_OPERATOR_ADD || nextToken.type == TOKEN_OPERATOR_SUB || nextToken.type == TOKEN_OPERATOR_GREATER_THAN || nextToken.type == TOKEN_OPERATOR_DOUBLE_EQUAL) {
                readToken(true, false);
                return value();
            }
        }
    }
    return false;
}

bool relationalExpression() {
    if (value()) {
        if (readToken(true, true)) {
            if (nextToken.type == TOKEN_SEMICOLON || nextToken.type == TOKEN_RPARAN) {
                return true;
            }
            if (nextToken.type == TOKEN_OPERATOR_GREATER_THAN || nextToken.type == TOKEN_OPERATOR_DOUBLE_EQUAL) {
                readToken(true, false);
                return relationalExpression();
            }
        }
    }
    return false;
}

bool parseExpression() {
    if (relationalExpression()) {
        if (readToken(true, true)) {
            if (nextToken.type == TOKEN_SEMICOLON) {
                return true;
            }
            if (nextToken.type == TOKEN_OPERATOR_DOUBLE_EQUAL) {
                return parseExpressionWithoutSemiColon();
            }
        }
    }

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

bool parseAssignmentWithoutSemiColon() {
    if (readToken(true, false) && nextToken.type == TOKEN_VARIABLE) {
        if (readToken(true, false) && nextToken.type == TOKEN_OPERATOR_EQUAL) {
            if (parseExpressionWithoutSemiColon()) {
                return true;
            }
        }
    }
    return false;
}

bool parseStatement();

bool parseForLoop() {
    if (readToken(true, false) && nextToken.type == TOKEN_FOR) {
        if (readToken(true, false) && nextToken.type == TOKEN_LPARAN) {
            if (parseAssignment()) {
                if (parseExpression()) {
                    readToken(true, false);
                    if (parseAssignmentWithoutSemiColon()) {
                        if (readToken(true, false) && nextToken.type == TOKEN_RPARAN) {
                            if (readToken(true, false) && nextToken.type == TOKEN_LBRACE) {
                                bool statements = true;
                                while (readToken(true, true) && nextToken.type != TOKEN_RBRACE && statements) {
                                    statements = parseStatement();
                                }
                                if (readToken(true, false) && nextToken.type == TOKEN_RBRACE && statements) {
                                    if (readToken(true, false) && nextToken.type == TOKEN_SEMICOLON) {
                                        return true;
                                    }
                                }
                            }
                        }
                    }
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
