#include "AnalyticTableaux.cpp"
#include <iostream>

using namespace std;
char findOperator(string &expr, int *local){
    if(expr[0] == '~'){
        return '~';
    }
    int pe = 0, pd = 0;
    for(int i=1; i<expr.size(); i++){
        if(expr[i] == '('){
            pe++;
        }
        else if(expr[i] == ')'){
            pd++;
        }
        else if(expr[i] == '&' || expr[i] == 'v' || expr[i] == '>'){
            if(pd == pe){
                *local = i;
                return expr[i];
            }
        }
    }
    return '-';
}  
string getLeft(string &expr, int index){
    string q = "";
    for(int i=1; i<index-1; i++){
        q += expr[i];
    }
    return q;
}
string getRight(string &expr, int index){
    string q = "";
    for(int i=index+2; i<expr.size() - 1; i++){
        q += expr[i];
    }
    return q;
}
string negado(string &expr){
    string a = "";
    for(int i=1; i<expr.size() - 1; i++){
        a += expr[i];
    }
    return a;
}
vector<Node*> applyRule(Node* node){
    vector<Node*> bla;
    int index;
    string expr = node->getExpression();
    char operador = findOperator(expr, &index);
    if(operador == '~'){
        bla = node->insertFront(negado(expr),!node->getTruthValue());
    }
    else if(operador != '-'){
        string P = getLeft(expr, index);
        string Q = getRight(expr, index);
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
    }
    else{
        string atom;
        atom += expr[2];
        bla = node->insertFront(atom, !node->getTruthValue());
    }
    node->markApplied();
    return bla;
}
void checkContradictions(vector<Node*> insertedNodes){
    for(int i=0; i<insertedNodes.size(); i++){
        if(insertedNodes[i]->checkContradiction()){
            insertedNodes[i]->markContradiction();
        }
    }
}
int getQuestion(string &linha, vector<string> &expr){
    string expr1, pergunta;
    for(int i=0; i<linha.size(); i++){
        if(linha[i + 1] != 'e'){
            expr1 += linha[i];
        }
        else{
            break;
        }
    }
    expr.push_back(expr1);
    for(int i=linha.size() - 15;i<linha.size(); i++){
        if(linha[i] != ' ' && linha[i] != ')'){
            pergunta += linha[i];
        }
    }
    if(pergunta == "etautologia?"){
        return 0;
    }
    else if(pergunta == "esatisfativel?"){
        return 1;
    }
    else if(pergunta == "erefutavel?"){
        return 2;
    }
    else if(pergunta == "insatisfativel?"){
        return 3;
    }
    else{
        int comecou = 0;
        expr1 = "";
        for(int i=0; i<linha.size(); i++){
            if(comecou && linha[i] != ',' && linha[i] != '}' && linha[i - 1] != ','){
                expr1 += linha[i];
            }
            if(linha[i] == '{'){
                comecou = 1;
            }
            if(linha[i] == '}'){
                expr.push_back(expr1);
                break;
            }
            if(linha[i] == ','){
                expr.push_back(expr1);
                expr1 = "";
            }
        }
        return 4;
    }
}
int main(){
    string linha;
    getline(cin, linha);
    vector<string> expr;
    int quest = getQuestion(linha, expr);
    if(quest == 0){//Tautologia
        Node tableua = Node(expr[0], 0);
        vector<Node*> apNodes = tableua.getAppliableNodes();
        vector<Node*> inNodes;
        while(!tableua.isClosed() && !apNodes.empty()){
            for(int i=0; i<apNodes.size(); i++){
                inNodes = applyRule(apNodes[i]);
                checkContradictions(inNodes);
            }
            apNodes = tableua.getAppliableNodes();
        }
        if(tableua.isClosed()){
            cout << "Sim, e tautologia." << endl;
        }
        else{
            cout << "Nao, nao e tautologia" << endl;
        }
    }
    else if(quest == 1){//Satisfativel
        Node tableua = Node(expr[0], 1);
        vector<Node*> apNodes = tableua.getAppliableNodes();
        vector<Node*> inNodes;
        while(!tableua.isClosed() && !apNodes.empty()){
            for(int i=0; i<apNodes.size(); i++){
                inNodes = applyRule(apNodes[i]);
                checkContradictions(inNodes);
            }
            apNodes = tableua.getAppliableNodes();
        }
        if(tableua.isClosed()){
            cout << "Nao, nao e satisfastivel." << endl;
        }
        else{
            cout << "Sim, e satisfativel" << endl;
        }
    }
    else if(quest == 2){//Refutavel
        Node tableua = Node(expr[0], 0);
        vector<Node*> apNodes = tableua.getAppliableNodes();
        vector<Node*> inNodes;
        while(!tableua.isClosed() && !apNodes.empty()){
            for(int i=0; i<apNodes.size(); i++){
                inNodes = applyRule(apNodes[i]);
                checkContradictions(inNodes);
            }
            apNodes = tableua.getAppliableNodes();
        }
        if(tableua.isClosed()){
            cout << "Nao, nao e refutavel." << endl;
        }
        else{
            cout << "Sim, e refutavel" << endl;
        }
    }
    else if(quest == 3){//Insatisfativel
        Node tableua = Node(expr[0], 1);
        vector<Node*> apNodes = tableua.getAppliableNodes();
        vector<Node*> inNodes;
        while(!tableua.isClosed() && !apNodes.empty()){
            for(int i=0; i<apNodes.size(); i++){
                inNodes = applyRule(apNodes[i]);
                checkContradictions(inNodes);
            }
            apNodes = tableua.getAppliableNodes();
        }
        if(tableua.isClosed()){
            cout << "Sim, e insatisfativel." << endl;
        }
        else{
            cout << "Nao, nao e insatisfativel" << endl;
        }
    }
    else{//Consequencia Logica
        Node tableua = Node(expr[0], 0);
        for(int i=1; i<expr.size(); i++){
            tableua.insertFront(expr[i], 1);
        }
        vector<Node*> apNodes = tableua.getAppliableNodes();
        vector<Node*> inNodes;
        while(!tableua.isClosed() && !apNodes.empty()){
            for(int i=0; i<apNodes.size(); i++){
                inNodes = applyRule(apNodes[i]);
                checkContradictions(inNodes);
            }
            apNodes = tableua.getAppliableNodes();
        }
        if(tableua.isClosed()){
            cout << "Sim, e conseguencia logica." << endl;
        }
        else{
            cout << "Nao, nao e consequencia logica" << endl;
        }
    }
    return 0;
}