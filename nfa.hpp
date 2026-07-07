#include <vector>
#include "ast.hpp"

struct Edge{
    char label;
    bool epsilon;
    bool any;
    State* to;
};

struct State{
    std::vector<Edge> edges;
};

struct Fragment{
    State* start;
    State* end;
};

class NFA{
    private:
    std::vector<std::unique_ptr<State>> pool;
    State* makeState();

    Fragment buildLiteral(LiteralNode*);
    Fragment buildDot(DotNode*);
    Fragment buildStar(StarNode*);
    Fragment buildConcat(ConcatNode*);
    Fragment buildGroup(GroupNode*);
    Fragment buildBracket(SquarebrackNode*);
    Fragment buildBackref(BackrefNode*);

    public:
    Fragment build(Node*);
};