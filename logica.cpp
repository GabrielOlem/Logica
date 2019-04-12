#include "AnalyticTableaux.cpp"
#include <iostream>

using namespace std;
vector<Node*> applyRule(Node* node){
    vector<Node*> bla;
    if(node->getExpression() == "((~Q) > (~P))" && node->getTruthValue() == 1){
        bla = node->insertSides("(~Q)", 0, "(~P)", 1);
    }
    else if(node->getExpression() == "(~Q)" && node->getTruthValue() == 0){
        bla = node->insertFront("Q", 1);
    }
    else if(node->getExpression() == "(~P)" && node->getTruthValue() == 1){
        bla = node->insertFront("P", 0);
    }
    node->markApplied();
    return bla;
}
void checkContradictions(vector<Node*> insertedNodes){
    for(int i=0; i< insertedNodes.size(); i++){
        if(insertedNodes[i]->checkContradiction()){
            insertedNodes[i]->markContradiction();
        }
    }
}
int main(){

    Node tableua = Node("((~Q) > (~P))", 1);
    tableua.insertFront("P", 1);
    tableua.insertFront("Q", 0);
    vector<Node*> apNodes = tableua.getAppliableNodes();
    vector<Node*> inNodes;
    while(!tableua.isClosed() && !apNodes.empty()){
        for(int i=0; i<apNodes.size(); i++){
            inNodes = applyRule(apNodes[i]);
            checkContradictions(inNodes);
        }
        apNodes = tableua.getAppliableNodes();
    }
    tableua.printTree();
    if(tableua.isClosed()){
        cout << "Fechado" << endl;
    }
    return 0;
}