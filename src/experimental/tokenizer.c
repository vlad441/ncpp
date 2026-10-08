#include <stdio.h>
#include <ctype.h>
#include <string.h>

//1. Лексер (Лексический анализатор / токенизатор)
//2. Парсер (Синтаксический анализатор)
//3. AST

//1. Лексер (Лексический анализатор / токенизатор)
typedef enum {
    TOKEN_EOF = 0,
	TOKEN_KEYWORD,
    TOKEN_IDENTIFIER, //var name
	TOKEN_OPERATOR, //OPERATOR_ASSIGN?
    TOKEN_NUMBER, //TOKEN_LITERAL?
    TOKEN_STRING,
    TOKEN_PUNCTUATOR
} TokenType;

typedef struct { TokenType type; const char* start; size_t len; int line, column; } Token;

static const char* keywords[] = { //C
	"void", "char", "short", "int", "long", "float", "double", "signed", "unsigned", "const",
	"if", "else", "switch", "case", "break", "continue", "default", "for", "while", "do", 
	"sizeof", "typedef", "struct", "enum", "union", "extern", "return",
	"volatile", "static", "goto", "register", "auto", NULL, //C++ //"class",
};

char is_keyword(const char* c, size_t len){ int i = 0;
    while(keywords[i] != NULL){ if(strlen(keywords[i]) == len && strncmp(keywords[i], c, len) == 0){ return 1; } i++; }
	return 0; }

Token next_token(const char* src, size_t len, size_t* currPos, int* line, int* col){
    Token tok; char c;

    while(*currPos < len){ c = src[*currPos];
        if(c == '\n'){ (*line)++; *col = 1; (*currPos)++; continue; } //новая строка
        if(isspace((unsigned char)c)){ (*col)++; (*currPos)++; continue; } //пробелы
        if(c == '/' && *currPos+1 < len && src[*currPos+1] == '/'){ while(*currPos < len && src[*currPos] != '\n'){ (*currPos)++; } continue; } //однострочные комментарии
        break; }

    if(*currPos >= len){ tok.type = TOKEN_EOF; tok.start = &src[len]; tok.len = 0; tok.line = *line; tok.column = *col; return tok; }

    c = src[*currPos]; tok.start = &src[*currPos]; tok.line = *line; tok.column = *col;

    if(isalpha((unsigned char)c) || c == '_'){
        size_t startPos = *currPos;
        while(*currPos < len && (isalnum((unsigned char)src[*currPos]) || src[*currPos] == '_')){
            (*currPos)++; (*col)++;
        }
        tok.len = *currPos - startPos;
        tok.type = is_keyword(tok.start, tok.len) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        return tok;
    }

    if(isdigit((unsigned char)c)){ size_t startPos = *currPos;
        while (*currPos < len && isalnum((unsigned char)src[*currPos])){ (*currPos)++; (*col)++; }
        tok.len = *currPos - startPos; tok.type = TOKEN_NUMBER; return tok;
    }

    if(c == '"' || c == '\''){
		char quote = c;
        size_t startPos = *currPos;
        (*currPos)++;
        (*col)++;
        while(*currPos < len){
            if(src[*currPos] == '\\' && *currPos + 1 < len){ *currPos += 2; *col += 2; continue; }
            if(src[*currPos] == quote){ (*currPos)++; (*col)++; break; }
            if(src[*currPos] == '\n'){ (*line)++; *col = 1; } else { (*col)++; }
            (*currPos)++;
        }
        tok.len = *currPos - startPos; tok.type = TOKEN_STRING; return tok;
    }

    if((c == '+' && *currPos + 1 < len && (src[*currPos + 1] == '+' || src[*currPos + 1] == '=')) || // ++, +=
        (c == '-' && *currPos + 1 < len && (src[*currPos + 1] == '-' || src[*currPos + 1] == '=' || src[*currPos + 1] == '>')) || // --, -=, ->
        (c == '=' && *currPos + 1 < len && src[*currPos + 1] == '=') || // ==
        (c == '!' && *currPos + 1 < len && src[*currPos + 1] == '=') || // !=
        (c == '<' && *currPos + 1 < len && (src[*currPos + 1] == '=' || src[*currPos + 1] == '<')) || // <=, <<
        (c == '>' && *currPos + 1 < len && (src[*currPos + 1] == '=' || src[*currPos + 1] == '>')) || // >=, >>
        (c == '&' && *currPos + 1 < len && (src[*currPos + 1] == '&' || src[*currPos + 1] == '=')) || // &&, &=
        (c == '|' && *currPos + 1 < len && (src[*currPos + 1] == '|' || src[*currPos + 1] == '=')) || // ||, |=
        (c == '*' && *currPos + 1 < len && src[*currPos + 1] == '=') || // *=
        (c == '/' && *currPos + 1 < len && src[*currPos + 1] == '=') || // /=
        (c == '%' && *currPos + 1 < len && src[*currPos + 1] == '=') || // %=
        (c == '^' && *currPos + 1 < len && src[*currPos + 1] == '='))   // ^=
	{	tok.len = 2; *currPos += 2; *col += 2; tok.type = TOKEN_OPERATOR; return tok; }

    tok.len = 1; (*currPos)++; (*col)++;
    if(strchr("+-*/%=<>!&|^.~?:#", c) != NULL){ tok.type = TOKEN_OPERATOR; }
	else if(strchr("{},;()[]", c) != NULL){ tok.type = TOKEN_PUNCTUATOR; } else{ /*ошибка*/ } return tok;
}

void tokenizer(){}

//2. Парсер (Синтаксический анализатор)
// ...
// ========= AST =========
//int main() { return 42; }
//AST_FUNC_DECL, AST_RETURN_STMT, AST_LITERAL

//x = 5 + 3 [Идентификатор(x), Оператор(=), Число(5), Оператор(+), Число(3)]