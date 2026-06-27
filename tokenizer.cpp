#include "tokenizer.hpp"
#include <iostream>
#include <string>

int pos;
std:: string pattern;

bool atEnd(){
    return pos >= pattern.size();
}

char peek(int offset){ // offset is how far you want to peek
    int p = pos + offset;
    return (p < pattern.size()) ? pattern[p] : '\0';
}

char consume() {
    return pattern[pos++];
}

bool dollarIsAnchor(){
    if (atEnd()) return true;                              
    return (pattern[pos] == '\\' && pos + 1 < pattern.size() && pattern[pos + 1] == ')');
}

Token handleEscape(int startPos) {
    char c = consume();
    switch (c) {
        case '(':
            return {TokType::LBRACK, '\0', {}, startPos};

        case ')':
            return {TokType::RBRACK, '\0', {}, startPos};

        case '{':
            return {TokType::LCURLY, '\0', {}, startPos};
        case '}': 
            return {TokType::RCURLY, '\0', {}, startPos};

        case '1': return {TokType::BACKREF, c, {}, startPos};
        case '2': return {TokType::BACKREF, c, {}, startPos};
        case '3': return {TokType::BACKREF, c, {}, startPos};
        case '4': return {TokType::BACKREF, c, {}, startPos};
        case '5': return {TokType::BACKREF, c, {}, startPos};
        case '6': return {TokType::BACKREF, c, {}, startPos};
        case '7': return {TokType::BACKREF, c, {}, startPos};
        case '8': return {TokType::BACKREF, c, {}, startPos};
        case '9': return {TokType::BACKREF, c, {}, startPos};

        default:
            return {TokType::LITERAL, c, {}, startPos};
    }
}

std::string BracketExpr(int startPos) {
    std::string raw = "[";

    if (!atEnd() && peek(0) == '^') {
        raw += consume();
    }
    if (!atEnd() && peek(0) == ']') {
        raw += consume();
    }

    while (true) {
        char c = consume();
        raw += c;

        if (c == ']')
            break;

    }
    return raw;
}

std::vector<Token> tokenize() {
    std::vector<Token> tokens;

    bool atStart = true;

    while (!atEnd()) {
        int startPos = pos;
        char c = consume();
        switch (c) {
            case '^':
                if (atStart){
                    tokens.push_back(Token{TokType::ANCHOR_START, '\0', {}, startPos});
                }
                else tokens.push_back(Token{TokType::LITERAL, '^',  {}, startPos});

                atStart = false;
                break;

            case '$':{ 
                bool anchor = dollarIsAnchor();
                if (anchor){
                    tokens.push_back(Token{TokType::ANCHOR_END, '\0', {}, startPos});
                }
                else tokens.push_back(Token{TokType::LITERAL, '$', {}, startPos});

                atStart = false;
                break;
            }

            case '.':
                tokens.push_back({TokType::DOT, '\0', {}, startPos});
                atStart = false;
                break;

            case '*':
                tokens.push_back({TokType::STAR, '\0', {}, startPos});
                atStart = false;
                break;

            case '[': {
                std::string raw = BracketExpr(startPos);
                tokens.push_back({TokType::SQUARE_BRACK, '\0', std::move(raw), startPos});
                atStart = false;
                break;
            }

            case '\\': {
                Token t = handleEscape(startPos);
                atStart = (t.type == TokType::LBRACK);
                tokens.push_back(std::move(t));
                break;
            }

            default:
                tokens.push_back({TokType::LITERAL, c, {}, startPos});
                atStart = false;
                break;
        }
    }
    tokens.push_back({TokType::END, '\0', {}, pos});
    return tokens;
}

const char* tokTypeStr(TokType t) {
    switch (t) {
        case TokType::LITERAL:       return "LITERAL";
        case TokType::DOT:           return "DOT";
        case TokType::STAR:          return "STAR";
        case TokType::ANCHOR_START:  return "ANCHOR_START";
        case TokType::ANCHOR_END:    return "ANCHOR_END";
        case TokType::LBRACK:        return "LBRACK";
        case TokType::RBRACK:        return "RBRACK";
        case TokType::LCURLY:        return "LCURLY";
        case TokType::RCURLY:        return "RCURLY";
        case TokType::SQUARE_BRACK:  return "SQUARE_BRACK";
        case TokType::BACKREF:       return "BACKREF";
        case TokType::END:           return "END";
    }
    return "0";
}

int main(){
    pattern = "^abc$d^[ab]$";
    std::vector<Token> tokens = tokenize();
    for(auto x : tokens) std::cout << tokTypeStr(x.type) << "\n";
}