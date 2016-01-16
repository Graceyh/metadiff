//
// Created by alex on 15/12/15.
//

#ifndef METADIFF_OPERATORS_LOGICAL_H
#define METADIFF_OPERATORS_LOGICAL_H

namespace metadiff {

    class LogicalBinary : public ElementwiseBinary{
    public:
        LogicalBinary(std::string const name,
                      GraphInPtr graph,
                      Node parent1,
                      Node parent2):
                ElementwiseBinary(name, graph, parent1, parent2)
        {};

        ad_value_type get_value_type(){
            return BOOLEAN;
        };

        ad_node_type get_node_type(){
            if(parent1.ptr->type == CONSTANT and parent2.ptr->type == CONSTANT){
                return CONSTANT;
            } else {
                return CONSTANT_DERIVED;
            }
        };

        Node get_parent_grad(Node my_grad, size_t index){
            return my_grad;
        }
    };

    class LogicalUnary : public UnaryOperator{
    public:
        LogicalUnary(std::string const name,
                     GraphInPtr graph,
                     Node parent):
                UnaryOperator(name, graph, parent)
        {};

        ad_value_type get_value_type(){
            return BOOLEAN;
        };

        ad_node_type get_node_type(){
            if(parent.ptr->type == CONSTANT){
                return CONSTANT;
            } else {
                return CONSTANT_DERIVED;
            }
        };

        Node get_parent_grad(Node my_grad, size_t index){
            return my_grad;
        }
    };

    class GreaterThan : public LogicalBinary {
    public:
        GreaterThan(GraphInPtr graph,
                    Node parent1,
                    Node parent2) :
                LogicalBinary("Gt", graph, parent1, parent2)
        {};
    };

    Node gt(Node node1, Node node2){
        return apply<GreaterThan>(node1, node2);
    }

    Node Node::gt(Node node) {
        return apply<GreaterThan>(this, node);
    }

    Node operator>(Node node1, Node node2){
        return apply<GreaterThan>(node1, node2);
    }

    class GreaterThanOrEqual : public LogicalBinary {
    public:
        GreaterThanOrEqual(GraphInPtr graph,
                           Node parent1,
                           Node parent2) :
                LogicalBinary("Ge", graph, parent1, parent2)
        {};
    };

    Node Node::ge(Node node) {
        return apply<GreaterThanOrEqual>(this, node);
    }

    Node ge(Node node1, Node node2){
        return apply<GreaterThanOrEqual>(node1, node2);
    }

    Node operator>=(Node node1, Node node2){
        return apply<GreaterThanOrEqual>(node1, node2);
    }

    class LessThan : public LogicalBinary {
    public:
        LessThan(GraphInPtr graph,
                 Node parent1,
                 Node parent2) :
                LogicalBinary("Lt", graph, parent1, parent2)
        {};
    };

    Node Node::lt(Node node) {
        return apply<LessThan>(this, node);
    }

    Node lt(Node node1, Node node2){
        return apply<LessThan>(node1, node2);
    }

    Node operator<(Node node1, Node node2){
        return apply<LessThan>(node1, node2);
    }

    class LessThanOrEqual : public LogicalBinary {
    public:
        LessThanOrEqual(GraphInPtr graph,
                        Node parent1,
                        Node parent2) :
                LogicalBinary("Le", graph, parent1, parent2)
        {};
    };

    Node Node::le(Node node) {
        return apply<LessThanOrEqual>(this, node);
    }

    Node le(Node node1, Node node2){
        return apply<LessThanOrEqual>(node1, node2);
    }

    Node operator<=(Node node1, Node node2){
        return apply<LessThanOrEqual>(node1, node2);
    }

    class Equals : public LogicalBinary {
    public:
        Equals(GraphInPtr graph,
               Node parent1,
               Node parent2) :
                LogicalBinary("Eq", graph, parent1, parent2)
        {};
    };

    Node Node::eq(Node node){
        return apply<Equals>(this, node);
    }

    Node eq(Node node1, Node node2){
        return apply<Equals>(node1, node2);
    }

    Node operator==(Node node1, Node node2){
        return apply<Equals>(node1, node2);
    }

    class NotEquals : public LogicalBinary {
    public:
        NotEquals(GraphInPtr graph,
                  Node parent1,
                  Node parent2) :
                LogicalBinary("Ne", graph, parent1, parent2)
        {};
    };

    Node Node::neq(Node node){
        return apply<NotEquals>(this, node);
    }

    Node neq(Node node1, Node node2){
        return apply<NotEquals>(node1, node2);
    }

    Node operator!=(Node node1, Node node2){
        return apply<NotEquals>(node1, node2);
    }

    class ApproximatelyEquals : public LogicalBinary {
    public:
        double tol;
        ApproximatelyEquals(GraphInPtr graph,
                            Node parent1,
                            Node parent2,
                            double tol) :
                LogicalBinary("ApproxEq", graph, parent1, parent2),
                tol(tol)
        {};
    };

    Node Node::approx_eq(Node node, double tol){
        GraphInPtr graph = ptr->graph;
        return graph->derived_node(std::make_shared<ApproximatelyEquals>(graph, this, node, tol));
    }

    Node approx_eq(Node node1, Node node2, double tol = 0.00001){
        GraphInPtr graph = node1.ptr->graph;
        return graph->derived_node(std::make_shared<ApproximatelyEquals>(graph, node1, node2, tol));
    }

    class ApproximatelyNotEquals : public LogicalBinary {
    public:
        double tol;
        ApproximatelyNotEquals(GraphInPtr graph,
                               Node parent1,
                               Node parent2,
                               double tol) :
                LogicalBinary("ApproxNe", graph, parent1, parent2),
                tol(tol)
        {};
    };

    Node Node::approx_neq(Node node, double tol){
        GraphInPtr graph = ptr->graph;
        return graph->derived_node(std::make_shared<ApproximatelyEquals>(graph, this, node, tol));
    }

    Node approx_neq(Node node1, Node node2, double tol=0.00001){
        GraphInPtr graph = node1.ptr->graph;
        return graph->derived_node(std::make_shared<ApproximatelyNotEquals>(graph, node1, node2, tol));
    }

    class And : public LogicalBinary {
    public:
        And(GraphInPtr graph,
            Node parent1,
            Node parent2) :
                LogicalBinary("And", graph, parent1, parent2)
        {
            if(parent1.ptr->v_type != BOOLEAN or parent2.ptr->v_type != BOOLEAN){
                throw UnknownError({parent1, parent2}, "Operator 'And' accepts only BOOLEAN inputs");
            }
        };
    };

    Node Node::logical_and(Node node){
        return apply<And>(this, node);
    }

    Node logical_and(Node node1, Node node2){
        return apply<And>(node1, node2);
    }

    Node operator&&(Node node1, Node node2){
        return apply<And>(node1, node2);
    }

    class Or : public LogicalBinary {
    public:
        Or(GraphInPtr graph,
           Node parent1,
           Node parent2) :
                LogicalBinary("Or", graph, parent1, parent2)
        {
            if(parent1.ptr->v_type != BOOLEAN or parent2.ptr->v_type != BOOLEAN){
                throw UnknownError({parent1, parent2}, "Operator 'Or' accepts only BOOLEAN inputs");
            }
        };
    };

    Node Node::logical_or(Node node){
        return apply<Or>(this, node);
    }

    Node logical_or(Node node1, Node node2){
        return apply<Or>(node1, node2);
    }

    Node operator||(Node node1, Node node2){
        return apply<Or>(node1, node2);
    }

    class ZeroElements : public LogicalUnary {
    public:
        ZeroElements(GraphInPtr graph,
                     Node parent) :
                LogicalUnary("ZeroElem", graph, parent)
        {};
    };

    Node Node::zero_elem() {
        return apply<ZeroElements>(this);
    }

    Node zero_elem(Node node){
        return apply<ZeroElements>(node);
    }

    class NonZeroElements : public LogicalUnary {
    public:
        NonZeroElements(GraphInPtr graph,
                        Node parent) :
                LogicalUnary("NonZeroElem", graph, parent)
        {};
    };

    Node Node::non_zero_elem() {
        return apply<NonZeroElements>(this);
    }

    Node non_zero_elem(Node node){
        return apply<NonZeroElements>(node);
    }

    class IsNaN : public LogicalUnary {
    public:
        IsNaN(GraphInPtr graph,
              Node parent) :
                LogicalUnary("IsNaN", graph, parent)
        {};
    };

    Node Node::is_nan(){
        return apply<IsNaN>(this);
    }

    Node is_nan(Node node){
        return apply<IsNaN>(node);
    }

    class IsInf : public LogicalUnary {
    public:
        IsInf(GraphInPtr graph,
              Node parent) :
                LogicalUnary("IsInf", graph, parent)
        {};
    };

    Node Node::is_inf(){
        return apply<IsInf>(this);
    }

    Node is_inf(Node node){
        return apply<IsInf>(node);
    }

    class Select: public BinaryOperator{
    public:
        Node condition;
        Select(GraphInPtr graph,
               Node condition,
               Node trueParent,
               Node falseParent):
                BinaryOperator("Select", graph, trueParent, falseParent),
                condition(condition)
        {
            shape = verify_elementwise_shapes(name, NodeVec{condition, trueParent, falseParent});
            if(condition.ptr->v_type != BOOLEAN){
                throw InvalidArguments(name, {condition, trueParent, falseParent},
                                       "The condition must have a value type BOOLEAN");
            }
            if(trueParent.ptr->v_type != falseParent.ptr->v_type){
                throw InvalidArguments(name, {condition, trueParent, falseParent},
                                       "The true and false statement must be of the same value type");
            }
            if(trueParent.is_constant()){
                this->parent1 = parent1.broadcast(shape);
            } else if(falseParent.is_constant()){
                this->parent1 = parent2.broadcast(shape);
            }
        };

        NodeVec get_arguments() {
            return NodeVec {condition};
        }

        Node get_parent_grad(Node my_grad, size_t index){
            Node zero = graph->constant_value(0.0);
            zero.ptr->grad_level = my_grad.ptr->grad_level;
            if(index == 0){
                return condition.select(my_grad, zero);
            } else {
                return condition.select(zero, my_grad);
            }
        };
    };

    Node Node::select(Node result_true, Node result_false){
        return ptr->graph->derived_node(std::make_shared<Select>(ptr->graph, this, result_true, result_false));
    }

    Node select(Node condition, Node result_true, Node result_false){
        return  condition.select(result_true, result_false);
    }
}
#endif //METADIFF_OPERATORS_LOGICAL_H
