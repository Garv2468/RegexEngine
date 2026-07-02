#include <vector>
#include <string>
#include "tokenizer.hpp"
#include "ast.hpp"

class Parser{
    private:
    int pos;
    int groupcount = 0;
    std::vector<Token> tokens;
    std::unique_ptr<Node> parseAtom();
    std::unique_ptr<Node> parsePiece();
    std::unique_ptr<Node> parseSequence();

    public:
    Parser(const std::vector<Token>& tokens): tokens(tokens) {}
    ParseResult parse();
};