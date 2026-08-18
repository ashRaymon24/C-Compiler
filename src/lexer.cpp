#include "lexer.h"

#include <cctype>

Lexer::Lexer(const std::string& source) : source_(source) {}

char Lexer::advance() {
    return source_[current_++];
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source_[current_];
}

char Lexer::peekNext() const {
    if (current_ + 1 >= source_.length()) return '\0';
    return source_[current_ + 1];
}

bool Lexer::isAtEnd() const {
    return current_ >= source_.length();
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line++;
                advance();
                break;
            default:
                return;
        }
    }
}

Token Lexer::makeToken(TokenType type) {
    std::string lexeme = source_.substr(current_ - 1, 1);
    return Token(type, lexeme, line);
}

Token Lexer::identifier() {
    size_t start = current_ - 1;
    while (isalnum(peek()) || peek() == '_') {
        advance();
    }
    std::string lexeme = source_.substr(start, current_ - start);
    TokenType type = TokenType::IDENTIFIER;
    if (lexeme == "int") type = TokenType::INT;
    else if (lexeme == "return") type = TokenType::RETURN;
    else if (lexeme == "if") type = TokenType::IF;
    else if (lexeme == "else") type = TokenType::ELSE;
    else if (lexeme == "while") type = TokenType::WHILE;
    else if (lexeme == "for") type = TokenType::FOR;
    return Token(type, lexeme, line);
}

Token Lexer::number() {
    size_t start = current_ - 1;
    while (isdigit(peek())) {
        advance();
    }
    std::string lexeme = source_.substr(start, current_ - start);
    return Token(TokenType::INTEGER, lexeme, line);
}

Token Lexer::scanToken() {
    skipWhitespace();
    if (isAtEnd()) return Token(TokenType::END_OF_FILE, "", line);

    char c = advance();
    switch (c) {
        case '(': return makeToken(TokenType::LEFT_PAREN);
        case ')': return makeToken(TokenType::RIGHT_PAREN);
        case '{': return makeToken(TokenType::LEFT_BRACE);
        case '}': return makeToken(TokenType::RIGHT_BRACE);
        case ';': return makeToken(TokenType::SEMICOLON);
        case '+': return makeToken(TokenType::PLUS);
        case '-': return makeToken(TokenType::MINUS);
        case '*': return makeToken(TokenType::STAR);
        case '/': return makeToken(TokenType::SLASH);
        case '=': {
            if (peek() == '=') {
                advance();
                return Token(TokenType::EQUALTO, "==", line);
            }
            return makeToken(TokenType::ASSIGN);
        }
        case ',': return makeToken(TokenType::COMMA);
        case '!': {
            if (peek() == '=') {
                advance();
                return Token(TokenType::NOTEQUALTO, "!=", line);
            }
            return makeToken(TokenType::NOT);
        }
        case '<': {
            if (peek() == '=') {
                advance();
                return Token(TokenType::LESSEQUALTO, "<=", line);
            }
            return makeToken(TokenType::LESSTHAN);
        }
        case '>': {
            if (peek() == '=') {
                advance();
                return Token(TokenType::GREATEREQUALTO, ">=", line);
            }
            return makeToken(TokenType::GREATERTHAN);
        }
        default:
            if (isalpha(c) || c == '_') {
                return identifier();
            } else if (isdigit(c)) {
                return number();
            } else {
                std::string lexeme(1, c);
                return Token(TokenType::UNKNOWN, lexeme, line);
            }
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (true) {
        Token token = scanToken();
        tokens.push_back(token);
        if (token.type == TokenType::END_OF_FILE) {
            break;
        }
    }
    return tokens;
}