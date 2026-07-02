#include "parser.hpp"
#include <vector>
#include "tokenizer.hpp"

std::unique_ptr<Node> Parser :: parseAtom(){
    if(pos >= tokens.size()) return nullptr;

    else if(tokens[pos].type == TokType::LITERAL){
        int startpos = tokens[pos].pos;
        char c = tokens[pos].ch;
        pos++;
        return std::make_unique<LiteralNode>(startpos, c);
    }

    else if(tokens[pos].type ==TokType::DOT){
        int startpos = tokens[pos].pos;
        pos++;
        return std::make_unique<DotNode>(startpos);
    }

    else if(tokens[pos].type == TokType::LBRACK){
        int startpos = tokens[pos].pos;
        int gnum = ++groupcount;
        pos++;
        std::unique_ptr<Node> res = parseSequence();
        bool aend = false;
        if(tokens[pos].type == TokType::ANCHOR_END) {pos++; aend = true;}
        pos++;
        return std::make_unique<GroupNode>(std::move(res), startpos, gnum, aend);
    }

    else if(tokens[pos].type == TokType::SQUARE_BRACK){
        std::vector <char> chars;
        int startpos = tokens[pos].pos;
        int i = 0;
        for(char x : tokens[pos].bracket) {
            if(x == '[' || x == ']' || (x == '^'&& i==0)) continue;
            else chars.push_back(x);
            i++;
        }
        pos++;
        if(tokens[pos-1].bracket[1] == '^') return std::make_unique<SquarebrackNode>(chars, startpos, true);

        return std::make_unique<SquarebrackNode>(chars, startpos, false);
    }
    else if(tokens[pos].type == TokType::BACKREF){
        int num = tokens[pos].ch -'0';
        int startpos = tokens[pos].pos;
        pos++;
        return std::make_unique<BackrefNode> (num, startpos);
    }
    else return nullptr;
}

std:: unique_ptr<Node> Parser :: parsePiece(){
    std:: unique_ptr<Node> res = parseAtom();
    if(res == nullptr) return nullptr;
    int startpos = tokens[pos].pos;
    if(tokens[pos].type == TokType::STAR){
        pos++;
        return std::make_unique<StarNode> (startpos, std::move(res));
    }
    else return res ;
}

std::unique_ptr<Node> Parser::parseSequence(){
    int startpos = tokens[pos].pos;
    std::vector<std::unique_ptr<Node>> parts;

    while(tokens[pos].type != TokType::END && tokens[pos].type != TokType::RBRACK){
        std::unique_ptr<Node> piece = parsePiece();
        if(piece == nullptr) break;
        parts.push_back(std::move(piece));
    }

    if(parts.size() == 1) return std::move(parts[0]);
    return std::make_unique<ConcatNode>(std::move(parts), startpos);
}

ParseResult Parser::parse(){
    ParseResult result;
    if(tokens[pos].type == TokType::ANCHOR_START){ result.anchorStart = true;pos++;}
    result.root = parseSequence();
    if(tokens[pos].type == TokType::ANCHOR_END) result.anchorEnd = true;
    return result;
}