#include "SyntaxNode.h"

void to_string(std::ostream &os, const std::shared_ptr<SyntaxNode> &node, std::string indent, bool isLast) {

    auto marker = isLast ? "└──" : "├──";
    os << indent;
    os << marker;
    os << node->Kind() << " " << node->GetValue();
    os << "\n";

    indent += isLast ? "    " : "│   ";

    auto children = node->GetChildren();
    if(children.empty())
        return;

    auto lastChild = children.back();

    for (auto &c: node->GetChildren()) {
        to_string(os, c, indent, c == lastChild);
    }


}

std::vector<std::shared_ptr<SyntaxNode>> NumberExpressionSyntax::GetChildren() {
    return {};
}

std::vector<std::shared_ptr<SyntaxNode>> UnaryOperator::GetChildren() {
    return {
        param,
    };
}

std::vector<std::shared_ptr<SyntaxNode>> BinaryOperator::GetChildren() {
    return {
        param1,
        param2
    };
}

std::vector<std::shared_ptr<SyntaxNode>> ParenthesisNode::GetChildren() {
    return {
        inside,
    };
}

std::vector<std::shared_ptr<SyntaxNode>> VariableDeclarationStatement::GetChildren() {
    return init.has_value() ?
    std::vector<std::shared_ptr<SyntaxNode>>{
        init.value(),
    } :
    std::vector<std::shared_ptr<SyntaxNode>>{};
}

std::vector<std::shared_ptr<SyntaxNode>> ExpressionStatement::GetChildren() {
    return {
        expression
    };
}

std::vector<std::shared_ptr<SyntaxNode>> AssignmentExpression::GetChildren() {
    return {
        param1,
        param2
    };
}
