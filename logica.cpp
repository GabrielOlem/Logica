#include "AnalyticTableaux.cpp"
#include <iostream>

using namespace std;
vector<Node*> applyRule(Node* node){
    vector<Node*> bla;
    /*
    Pega operador
    parte da esquerda , P, e parte da direita, Q.
    if(operador == 'v' && node->getTruthValue() == 0){
        bla = node->insertFront(P, 0);
        bla = node->insertFront(Q, 0);
    }
    else if(operador == '>' && node->getTruthValue() == 0){
        bla = node->insertFront(P, 1);
        bla = node->insertFront(Q, 0);
    }
    else if(operador == '&' && node->getTruthValue() == 1){
        bla = node->insertFront(P, 1);
        bla = node->insertFront(Q, 1);
    }
    else if(operador == 'v' && node->getTruthValue() == 1){
        bla = node->insertSides(P, 1, Q, 1);
    }
    else if(operador == '>' && node->getTruthValue() == 1){
        bla = node->insertSides(P, 0, Q, 1);
    }
    else if(operador == '&' && node->getTruthValue() == 0){
        bla = node->insertSides(P, 0, Q, 0);
    }
    */
    if(node->getExpression() == "(A v (~A))" && node->getTruthValue() == 0){
        cout << 'a' << endl;
        bla = node->insertFront("A", 0);
        bla = node->insertFront("(~A)", 0);
    }
    else if(node->getExpression() == "(~A)" && node->getTruthValue() == 0){
        bla = node->insertFront("A", 1);
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

    Node tableua = Node("(A v (~A))", 0);
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
        cout << "Isso é uma tautologia" << endl;
    }
    return 0;
}