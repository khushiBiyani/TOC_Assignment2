#include <stdarg.h>
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
    TOKEN_KEYWORD_INT,
    TOKEN_KEYWORD_FOR,
    TOKEN_KEYWORD_READ,
    TOKEN_KEYWORD_WRITE,
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
            nextToken.type = TOKEN_KEYWORD_INT;
        } else if (!strcmp(nextToken.data, "for")) {
            nextToken.type = TOKEN_KEYWORD_FOR;
        }
    } else if (nextToken.size == 4) {
        if (!strcmp(nextToken.data, "read")) {
            nextToken.type = TOKEN_KEYWORD_READ;
        }
    } else if (nextToken.size == 5) {
        if (!strcmp(nextToken.data, "write")) {
            nextToken.type = TOKEN_KEYWORD_WRITE;
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
    switch (nextToken.type) {
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
        case TOKEN_KEYWORD_INT: {
            printf("int");
            break;
        }
        case TOKEN_KEYWORD_FOR: {
            printf("for");
            break;
        }
        case TOKEN_KEYWORD_READ: {
            printf("read");
            break;
        }
        case TOKEN_KEYWORD_WRITE: {
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
             _               _   _            __  __
            | |             | | | |          / _|/ _|
  ___  _   _| |_ _ __  _   _| |_| |__  _   _| |_| |_ ___ _ __
 / _ \| | | | __| '_ \| | | | __| '_ \| | | |  _|  _/ _ \ '__|
| (_) | |_| | |_| |_) | |_| | |_| |_) | |_| | | | ||  __/ |
 \___/ \__,_|\__| .__/ \__,_|\__|_.__/ \__,_|_| |_| \___|_|
                | |
                |_|
*/

char outputBuffer[MAX];
int outputBufferLength;

void bprintf(char *str, ...) {
    va_list argptr;
    va_start(argptr, str);
    int ret = vsprintf(outputBuffer + outputBufferLength, str, argptr);
    if (ret < 0) ret = 0;
    outputBufferLength += ret;
    va_end(argptr);
}

void bPrintClose() {
    bprintf(" ]");
}

void bprintToken() {
    bprintf("[");
    switch (nextToken.type) {
        case INVALID_TOKEN: {
            break;
        }
        case TOKEN_SEMICOLON: {
            bprintf(";");
            break;
        }
        case TOKEN_COMMA: {
            bprintf(",");
            break;
        }
        case TOKEN_SPACE: {
            bprintf("SP");
            break;
        }
        case TOKEN_KEYWORD_INT: {
            bprintf("int");
            break;
        }
        case TOKEN_KEYWORD_FOR: {
            bprintf("for");
            break;
        }
        case TOKEN_KEYWORD_READ: {
            bprintf("read");
            break;
        }
        case TOKEN_KEYWORD_WRITE: {
            bprintf("write");
            break;
        }
        case TOKEN_OPERATOR_ADD: {
            bprintf("+");
            break;
        }
        case TOKEN_OPERATOR_SUB: {
            bprintf("-");
            break;
        }
        case TOKEN_OPERATOR_MUL: {
            bprintf("*");
            break;
        }
        case TOKEN_OPERATOR_DIV: {
            bprintf("/");
            break;
        }
        case TOKEN_OPERATOR_GREATER_THAN: {
            bprintf(">");
            break;
        }
        case TOKEN_OPERATOR_EQUAL: {
            bprintf("=");
            break;
        }
        case TOKEN_OPERATOR_DOUBLE_EQUAL: {
            bprintf("==");
            break;
        }
        case TOKEN_LPARAN: {
            bprintf("(");
            break;
        }
        case TOKEN_RPARAN: {
            bprintf(")");
            break;
        }
        case TOKEN_LBRACE: {
            bprintf("{");
            break;
        }
        case TOKEN_RBRACE: {
            bprintf("}");
            break;
        }
        case TOKEN_VARIABLE: {
            bprintf("var:{%s}", nextToken.data);
            break;
        }
        case TOKEN_INTEGER_LITERAL: {
            bprintf("INTC:{%s}", nextToken.data);
            break;
        }
    }
    bPrintClose();
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
    bprintf("[D ");
    if (readToken(true, false) && nextToken.type == TOKEN_KEYWORD_INT) {
        bprintToken();
        if (readToken(false, false) && nextToken.type == TOKEN_SPACE) {
            bprintToken();
            while (true) {
                if (readToken(true, false) && nextToken.type == TOKEN_VARIABLE) {
                    bprintToken();
                    if (readToken(true, false)) {
                        if (nextToken.type == TOKEN_COMMA) {
                            bprintToken();
                            continue;
                        } else if (nextToken.type == TOKEN_SEMICOLON) {
                            bprintToken();
                            break;
                        } else
                            return false;
                    }
                } else
                    return false;
            }
            bPrintClose();
            return true;
        }
    }
    return false;
}

bool parseRead() {
    bprintf("[D");
    if (readToken(true, false) && nextToken.type == TOKEN_KEYWORD_READ) {
        bprintToken();
        if (readToken(false, false) && nextToken.type == TOKEN_SPACE) {
            bprintToken();
            if (readToken(true, false) && nextToken.type == TOKEN_VARIABLE) {
                bprintToken();
                if (readToken(true, false) && nextToken.type == TOKEN_SEMICOLON) {
                    bprintToken();
                    bPrintClose();
                    return true;
                }
            }
        }
    }
    return false;
}

bool parseWrite() {
    bprintf("[W");
    if (readToken(true, false) && nextToken.type == TOKEN_KEYWORD_WRITE) {
        bprintToken();
        if (readToken(false, false) && nextToken.type == TOKEN_SPACE) {
            bprintToken();
            if (readToken(true, false) && (nextToken.type == TOKEN_VARIABLE || nextToken.type == TOKEN_INTEGER_LITERAL)) {
                bprintToken();
                if (readToken(true, false) && nextToken.type == TOKEN_SEMICOLON) {
                    bprintToken();
                    bPrintClose();
                    return true;
                }
            }
        }
    }
    return false;
}

bool relationalExpression();

bool parseExpressionWithoutSemiColon() {
    bprintf("[EWS");
    if (relationalExpression()) {
        if (readToken(true, true)) {
            if (nextToken.type == TOKEN_RPARAN) {
                bPrintClose();
                return true;
            }
            if (nextToken.type == TOKEN_OPERATOR_DOUBLE_EQUAL) {
                if (!parseExpressionWithoutSemiColon()) return false;
                bPrintClose();
                return true;
            }
        }
    }
    return false;
}

bool parseStatement();

bool parseFactor() {
    bprintf("[F");
    if (readToken(true, false)) {
        if (nextToken.type == TOKEN_VARIABLE || nextToken.type == TOKEN_INTEGER_LITERAL) {
            bprintToken();
            bPrintClose();
            return true;
        }
        if (nextToken.type == TOKEN_LPARAN) {
            bprintToken();
            if (parseExpressionWithoutSemiColon()) {
                if (readToken(true, false) && nextToken.type == TOKEN_RPARAN) {
                    bprintToken();
                    bPrintClose();
                    return true;
                }
            }
        }
    }
    return false;
}

bool term() {
    bprintf("[T");
    if (parseFactor()) {
        if (readToken(true, true)) {
            if (nextToken.type == TOKEN_SEMICOLON || nextToken.type == TOKEN_RPARAN) {
                bPrintClose();
                return true;
            }
            if (nextToken.type == TOKEN_OPERATOR_DIV || nextToken.type == TOKEN_OPERATOR_MUL || nextToken.type == TOKEN_OPERATOR_ADD || nextToken.type == TOKEN_OPERATOR_SUB || nextToken.type == TOKEN_OPERATOR_GREATER_THAN || nextToken.type == TOKEN_OPERATOR_DOUBLE_EQUAL) {
                readToken(true, false);
                bprintToken();
                if (!term()) return false;
                bPrintClose();
                return true;
            }
        }
    }
    return false;
}

bool value() {
    bprintf("[V");
    if (term()) {
        if (readToken(true, true)) {
            if (nextToken.type == TOKEN_SEMICOLON || nextToken.type == TOKEN_RPARAN) {
                bPrintClose();
                return true;
            }
            if (nextToken.type == TOKEN_OPERATOR_ADD || nextToken.type == TOKEN_OPERATOR_SUB || nextToken.type == TOKEN_OPERATOR_GREATER_THAN || nextToken.type == TOKEN_OPERATOR_DOUBLE_EQUAL) {
                readToken(true, false);
                bprintToken();
                if (!value()) return false;
                bPrintClose();
                return true;
            }
        }
    }
    return false;
}

bool relationalExpression() {
    bprintf("[RE");
    if (value()) {
        if (readToken(true, true)) {
            if (nextToken.type == TOKEN_SEMICOLON || nextToken.type == TOKEN_RPARAN) {
                bPrintClose();
                return true;
            }
            if (nextToken.type == TOKEN_OPERATOR_GREATER_THAN || nextToken.type == TOKEN_OPERATOR_DOUBLE_EQUAL) {
                readToken(true, false);
                bprintToken();
                if (!relationalExpression()) return false;
                bPrintClose();
                return true;
            }
        }
    }
    return false;
}

bool parseExpression() {
    bprintf("[E");
    if (relationalExpression()) {
        if (readToken(true, true)) {
            if (nextToken.type == TOKEN_SEMICOLON) {
                bPrintClose();
                return true;
            }
            if (nextToken.type == TOKEN_OPERATOR_DOUBLE_EQUAL) {
                if (!parseExpressionWithoutSemiColon()) return false;
                bPrintClose();
                return true;
            }
        }
    }
    return false;
}

bool parseAssignment() {
    bprintf("[A");
    if (readToken(true, false) && nextToken.type == TOKEN_VARIABLE) {
        bprintToken();
        if (readToken(true, false) && nextToken.type == TOKEN_OPERATOR_EQUAL) {
            bprintToken();
            if (parseExpression()) {
                if (readToken(true, false) && nextToken.type == TOKEN_SEMICOLON) {
                    bprintToken();
                    bPrintClose();
                    return true;
                }
            }
        }
    }
    return false;
}

bool parseAssignmentWithoutSemiColon() {
    bprintf("[AWS");
    if (readToken(true, false) && nextToken.type == TOKEN_VARIABLE) {
        bprintToken();
        if (readToken(true, false) && nextToken.type == TOKEN_OPERATOR_EQUAL) {
            bprintToken();
            if (parseExpressionWithoutSemiColon()) {
                bPrintClose();
                return true;
            }
        }
    }
    return false;
}

bool parseForLoop() {
    bprintf("[FOR");
    if (readToken(true, false) && nextToken.type == TOKEN_KEYWORD_FOR) {
        bprintToken();
        if (readToken(true, false) && nextToken.type == TOKEN_LPARAN) {
            bprintToken();
            if (parseAssignment()) {
                if (parseExpression()) {
                    readToken(true, false);
                    bprintToken();
                    if (parseAssignmentWithoutSemiColon()) {
                        if (readToken(true, false) && nextToken.type == TOKEN_RPARAN) {
                            bprintToken();
                            if (readToken(true, false) && nextToken.type == TOKEN_LBRACE) {
                                bprintToken();
                                bool statements = true;
                                while (readToken(true, true) && nextToken.type != TOKEN_RBRACE && statements) {
                                    statements = parseStatement();
                                }
                                if (readToken(true, false) && nextToken.type == TOKEN_RBRACE && statements) {
                                    bprintToken();
                                    if (readToken(true, false) && nextToken.type == TOKEN_SEMICOLON) {
                                        bprintToken();
                                        bPrintClose();
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
    bprintf("[S");
    if (readToken(true, true)) {
        bool ok;
        switch (nextToken.type) {
            case TOKEN_KEYWORD_READ: {
                ok = parseRead();
                break;
            }
            case TOKEN_KEYWORD_WRITE: {
                ok = parseWrite();
                break;
            }
            case TOKEN_KEYWORD_FOR: {
                ok = parseForLoop();
                break;
            }
            case TOKEN_VARIABLE: {
                ok = parseAssignment();
                break;
            }
            default: {
                return false;
            }
        }
        bPrintClose();
        return ok;
    } else {
        return false;
    }
}

bool parseFirstStatement() {
    bprintf("[FS");
    if (readToken(true, true)) {
        bool ok;
        switch (nextToken.type) {
            case TOKEN_KEYWORD_INT: {
                ok = parseDeclaration();
                break;
            }
            case TOKEN_KEYWORD_READ: {
                ok = parseRead();
                break;
            }
            case TOKEN_KEYWORD_WRITE: {
                ok = parseWrite();
                break;
            }
            case TOKEN_KEYWORD_FOR: {
                ok = parseForLoop();
                break;
            }
            case TOKEN_VARIABLE: {
                ok = parseAssignment();
                break;
            }
            default: {
                return false;
            }
        }
        bPrintClose();
        return ok;
    } else {
        return false;
    }
}

bool parseManyStatements() {
    bprintf("[MS");
    if (!parseStatement()) return false;
    if (canReadToken()) {
        if (!parseManyStatements()) return false;
    }
    bPrintClose();
    return true;
}

bool parseProgram() {
    bprintf("[P");
    if (!parseFirstStatement()) return false;
    if (canReadToken()) {
        if (!parseManyStatements()) return false;
    }
    bPrintClose();
    return true;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ./simulator <input.txt>\n");
        exit(1);
    }

    readCode(argv[1]);

    if (parseProgram()) {
        printf("%s\n", outputBuffer);
    } else {
        printf("Grammar is incorrect!\n");
    }
}
