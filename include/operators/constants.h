//
// Created by alex on 17/12/15.
//

#ifndef METADIFF_OPERATORS_CONSTANTS_H
#define METADIFF_OPERATORS_CONSTANTS_H

namespace metadiff{
    class MakeConstant: public UnaryOperator{
    public:
        MakeConstant(GraphInPtr graph,
                     Node parent):
                UnaryOperator("Const", graph, parent)
        {};

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

    Node Node::constant() {
        return apply<MakeConstant>(this);
    }
    
    class ConstantOperator: public Operator{
    public:
        Shape shape;
        ConstantOperator(std::string const name,
                         GraphInPtr graph):
                Operator(name, graph)
        {};

        NodeVec get_parents() {
            return {};
        };

        ad_value_type get_value_type(){
            return FLOAT;
        };

        ad_node_type get_node_type(){
            for(int i=0;i<4;i++){
                if(not shape[i].is_constant()){
                    return CONSTANT_DERIVED;
                }
            }
            return CONSTANT;
        };

        Shape get_shape(){
            return shape;
        }

        size_t get_gradient_level(){
            return 0;
        };

        NodeVec get_arguments() {
            return NodeVec {};
        }

        Node get_parent_grad(Node my_grad, size_t index){
            return my_grad;
        }
    };

    class Eye: public ConstantOperator{
    public:
        Eye(GraphInPtr graph, SymInt size):
                ConstantOperator("Eye", graph)
        {
            shape = {size, size, 1, 1};
        }
    };

    Node GraphInternal::eye(SymInt size) {
        return derived_node(std::make_shared<Eye>(this, size));
    }

    class Zeros: public ConstantOperator{
    public:
        Zeros(GraphInPtr graph, Shape shape):
                ConstantOperator("Zeros", graph)
        {
            this->shape = shape;
        }
    };

    Node GraphInternal::zeros(Shape shape) {
        return derived_node(std::make_shared<Zeros>(this, shape));
    }

    class Ones: public ConstantOperator{
    public:
        Ones(GraphInPtr graph, Shape shape):
                ConstantOperator("Ones", graph)
        {
            this->shape = shape;
        }
    };

    Node GraphInternal::ones(Shape shape) {
        return derived_node(std::make_shared<Ones>(this, shape));
    }

    class ConstantValue: public ConstantOperator{
    public:
        double value;
        ConstantValue(GraphInPtr graph, Shape shape, double value):
                ConstantOperator("Value", graph),
                value(value)
        {
            this->shape = shape;
        }
    };

    Node GraphInternal::constant_value(double value, Shape shape){
        if(value == 0.0){
            return zeros(shape);
        } else if(value == 1.0){
            return ones(shape);
        } else{
            return derived_node(std::make_shared<ConstantValue>(this, shape, value));
        }
    }

//    Node mul_const_operators(Node node1, Node node2){
//        if(node1->op->name == "Ones"){
//            return node2;
//        }
//        if(node1->op->name == "Zeros"){
//            return node1;
//        }
//        if(node2->op->name == "Ones"){
//            return node1;
//        }
//        if(node2->op->name == "Zeros"){
//            return node2;
//        }
//        if(node1->op->name == "Value" and node2->op->name == "Value"){
//            double value1 = dynamic_cast<ConstantValue*>(node1->op.get())->value;
//            double value2 = dynamic_cast<ConstantValue*>(node2->op.get())->value;
//            return node1->graph->value(value1 * value2);
//        }
//        Node result = mul(node1, node2);
//        if(node1->op->name == "Value"){
//            result->value = node->value *
//        }
//        result->value =
//    }
}
#endif //METADIFF_OPERATORS_CONSTANTS_H
