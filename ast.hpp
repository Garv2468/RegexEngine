#include <string>
#include <vector>
#include <memory>
#pragma once

enum class Nodekind{
    LITERAL_NODE, 
    STAR_NODE,
    DOT_NODE, 
    BACKREF_NODE,
    GROUP_NODE, 
    SQUARE_BRACK_NODE,
    CONCAT_NODE
};

struct Node{
    Nodekind kind;
    int pos;
    Node(Nodekind k, int p) : kind(k), pos(p) {}
    virtual ~Node() = default;
};

struct ParseResult {
    std::unique_ptr<Node> root;
    bool anchorStart = false;
    bool anchorEnd = false;
};

struct LiteralNode: public Node
{
    char ch;
    LiteralNode(int p, char c): Node(Nodekind::LITERAL_NODE, p), ch(c){}
};

struct StarNode : public Node
{
    std::unique_ptr<Node> op;
    StarNode(int p, std::unique_ptr<Node> s): Node(Nodekind::STAR_NODE, p), op(std::move(s)) {}
};

struct DotNode: public Node 
{
    DotNode(int p): Node(Nodekind::DOT_NODE, p) {}
};

struct BackrefNode: public Node
{
    int num;
    BackrefNode(int n, int p): Node(Nodekind::BACKREF_NODE, p), num(n) {}
};

struct GroupNode: public Node
{
    int groupnum;
    std::unique_ptr<Node> inner;
    GroupNode(std::unique_ptr<Node> c, int p, int n): Node(Nodekind::GROUP_NODE, p), inner(std::move(c)), groupnum(n) {}
};

struct SquarebrackNode: public Node
{
    std::vector<char> str;
    bool neg;
    SquarebrackNode(std::vector<char> s, int p, bool n): Node(Nodekind::SQUARE_BRACK_NODE, p), neg(n), str(std::move(s)) {}
};

struct ConcatNode : public Node {
    std::vector<std::unique_ptr<Node>> parts;
    ConcatNode(std::vector<std::unique_ptr<Node>> p, int pos) : Node(Nodekind::CONCAT_NODE, pos), parts(std::move(p)) {}
};
