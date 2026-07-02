#include <iostream>
#include "parser.hpp"

void printTree(Node* node, int indent = 0){
    std::string pad(indent, ' ');
    if(!node){ std::cout << pad << "null\n"; return; }
    
    switch(node->kind){
        case Nodekind::LITERAL_NODE:
            std::cout << pad << "Literal: " << static_cast<LiteralNode*>(node)->ch << "\n";
            break;
        case Nodekind::CONCAT_NODE:{
            std::cout << pad << "Concat\n";
            for(auto& child : static_cast<ConcatNode*>(node)->parts)
                printTree(child.get(), indent+2);
            break;
        }
        case Nodekind::GROUP_NODE:{
            auto* g = static_cast<GroupNode*>(node);
            std::cout << pad << "Group(" << g->groupnum << ")\n";
            printTree(g->inner.get(), indent+2);
            break;
        }
        case Nodekind::STAR_NODE:
            std::cout << pad << "Star\n";
            printTree(static_cast<StarNode*>(node)->op.get(), indent+2);
            break;
        case Nodekind::DOT_NODE:
            std::cout << pad << "Dot\n"; break;
        case Nodekind::BACKREF_NODE:
            std::cout << pad << "Backref: " << static_cast<BackrefNode*>(node)->num << "\n"; break;
        case Nodekind::SQUARE_BRACK_NODE:
            std::cout << pad << "Bracket\n"; break;
    }
}

int main(){
    Tokenizer t("^agdkj[dfd][fd*]\\(b$\\)\\(asj\\(ABA\\)fk\\)c");
    std::vector<Token> tokens = t.tokenize();
    //for(auto x : tokens) std::cout << tokTypeStr(x.type) << "\n";
    Parser p(tokens);
    ParseResult res = p.parse();
    printTree(res.root.get());
}