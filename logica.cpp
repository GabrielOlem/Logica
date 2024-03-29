#include "AnalyticTableaux.cpp"
#include <iostream>
#include <fstream>

using namespace std;
char findOperator(string &expr, int *local){
    if(expr[1] == '~'){
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
string get(string &expr, int esq, int dir){
    string q = "";
    for(int i=esq; i<dir; i++){
        q += expr[i];
    }
    return q;
}
string negado(string &expr){
    string q = "";
    for(int i=2; i<expr.size() - 1; i++){
        q += expr[i];
    }
    return q;
}
void checkContradictions(vector<Node*> insertedNodes){
    for(int i=0; i<insertedNodes.size(); i++){
        if(insertedNodes[i]->checkContradiction()){
            insertedNodes[i]->markContradiction();
        }
    }
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
        string P = get(expr, 1, index - 1);
        string Q = get(expr, index + 2, expr.size()-1);
        if(operador == 'v' && node->getTruthValue() == 0){
            bla = node->insertFront(P, 0);
            checkContradictions(bla);
            bla = node->insertFront(Q, 0);
        }
        else if(operador == '>' && node->getTruthValue() == 0){
            bla = node->insertFront(P, 1);
            checkContradictions(bla);
            bla = node->insertFront(Q, 0);
        }
        else if(operador == '&' && node->getTruthValue() == 1){
            bla = node->insertFront(P, 1);
            checkContradictions(bla);
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
    node->markApplied();
    return bla;
}

int getQuestion(string &linha, vector<string> &expr){
    string expr1, pergunta;
    int comecou = 0;
    for(int i=0; i<linha.size(); i++){
        if(linha[i + 1] != 'e'){
            expr1 += linha[i];
        }
        else{
            break;
        }
    }
    expr.push_back(expr1);
    if(linha[linha.size() - 15] == 'i'){
        return 3;
    }
    for(int i=0;i<linha.size(); i++){
        if(linha[i] == 'e'){
            comecou = 1;
        }
        if(linha[i] != ' ' && comecou){
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
bool isAlpha(string expr, int tv){
    int numero;
    char ope = findOperator(expr, &numero);
    if((ope == '&' && tv == 0) || (ope == 'v' && tv == 1) || (ope == '>' && tv == 1)){
        return false;
    }
    else{
        return true;
    }
}
Node* getAlpha(vector<Node*> apNodes, int indexorig){
    for(int i=0; i<apNodes.size(); i++){
        if(isAlpha(apNodes[i]->getExpression(), apNodes[i]->getTruthValue())){
            return apNodes[i];
        }
    }
    return apNodes[indexorig];
}
int main(){
    ifstream fioa("Entrada.in");
    ofstream saida("Saida.out");
    string linha;
    int n;
    fioa >> n;
    for(int i=0; i<=n; i++){
        getline(fioa, linha);
        if(i != 0){
            saida << "Problema #" << i << endl;
            vector<string> expr;
            int quest = getQuestion(linha, expr);
            if(quest == 0){//Tautologia
                Node tableua = Node(expr[0], 0);
                vector<Node*> apNodes = tableua.getAppliableNodes();
                vector<Node*> inNodes;
                while(!tableua.isClosed() && !apNodes.empty()){
                    for(int i=0; i<apNodes.size(); i++){
                        if(isAlpha(apNodes[i]->getExpression(), apNodes[i]->getTruthValue())){
                            inNodes = applyRule(apNodes[i]);
                            checkContradictions(inNodes);
                        }
                        else{
                            inNodes = applyRule(getAlpha(apNodes, i));
                            checkContradictions(inNodes);
                            
                        }
                        apNodes = tableua.getAppliableNodes();
                    }
                    
                }
                if(tableua.isClosed()){
                    saida << "Sim, e tautologia." << endl;
                }
                else{
                    saida << "Nao, nao e tautologia." << endl;
                }
            }
            else if(quest == 1){//Satisfativel
                Node tableua = Node(expr[0], 1);
                vector<Node*> apNodes = tableua.getAppliableNodes();
                vector<Node*> inNodes;
                while(!tableua.isClosed() && !apNodes.empty()){
                    for(int i=0; i<apNodes.size(); i++){
                        if(isAlpha(apNodes[i]->getExpression(), apNodes[i]->getTruthValue())){
                            inNodes = applyRule(apNodes[i]);
                            checkContradictions(inNodes);
                        }
                        else{
                            inNodes = applyRule(getAlpha(apNodes, i));
                            checkContradictions(inNodes);
                            
                        }
                        apNodes = tableua.getAppliableNodes();
                    }
                }
                if(tableua.isClosed()){
                    saida << "Nao, nao e satisfativel." << endl;
                }
                else{
                    saida << "Sim, e satisfativel." << endl;
                }
            }
            else if(quest == 2){//Refutavel
                Node tableua = Node(expr[0], 0);
                vector<Node*> apNodes = tableua.getAppliableNodes();
                vector<Node*> inNodes;
                while(!tableua.isClosed() && !apNodes.empty()){
                    for(int i=0; i<apNodes.size(); i++){
                        if(isAlpha(apNodes[i]->getExpression(), apNodes[i]->getTruthValue())){
                            inNodes = applyRule(apNodes[i]);
                            checkContradictions(inNodes);
                        }
                        else{
                            inNodes = applyRule(getAlpha(apNodes, i));
                            checkContradictions(inNodes);
                            
                        }
                        apNodes = tableua.getAppliableNodes();
                    }
                }
                if(tableua.isClosed()){
                    saida << "Nao, nao e refutavel." << endl;
                }
                else{
                    saida << "Sim, e refutavel." << endl;
                }
            }
            else if(quest == 3){//Insatisfativel
                Node tableua = Node(expr[0], 1);
                vector<Node*> apNodes = tableua.getAppliableNodes();
                vector<Node*> inNodes;
                while(!tableua.isClosed() && !apNodes.empty()){
                    for(int i=0; i<apNodes.size(); i++){
                        if(isAlpha(apNodes[i]->getExpression(), apNodes[i]->getTruthValue())){
                            inNodes = applyRule(apNodes[i]);
                            checkContradictions(inNodes);
                        }
                        else{
                            inNodes = applyRule(getAlpha(apNodes, i));
                            checkContradictions(inNodes);
                            
                        }
                        apNodes = tableua.getAppliableNodes();
                    }
                }
                if(tableua.isClosed()){
                    saida << "Sim, e insatisfativel." << endl;
                }
                else{
                    saida << "Nao, nao e insatisfativel." << endl;
                }
            }
            else{//Consequencia Logica
                int hasEqual = 0;
                Node tableua = Node(expr[0], 0);
                for(int i=1; i<expr.size(); i++){
                    tableua.insertFront(expr[i], 1);
                    if(expr[0] == expr[i]){
                        hasEqual = 1;
                    }
                }
                if(hasEqual == 1){
                    saida << "Sim, e consequencia logica." << endl;
                }
                else{
                    vector<Node*> apNodes = tableua.getAppliableNodes();
                    vector<Node*> inNodes;
                    while(!tableua.isClosed() && !apNodes.empty()){
                        for(int i=0; i<apNodes.size(); i++){
                            if(isAlpha(apNodes[i]->getExpression(), apNodes[i]->getTruthValue())){
                                inNodes = applyRule(apNodes[i]);
                                checkContradictions(inNodes);
                            }
                            else{
                                inNodes = applyRule(getAlpha(apNodes, i));
                                checkContradictions(inNodes);
                            }
                            apNodes = tableua.getAppliableNodes();
                        }
                    }
                    if(tableua.isClosed()){
                        saida << "Sim, e consequencia logica." << endl;
                    }
                    else{
                        saida << "Nao, nao e consequencia logica." << endl;
                    }
                }
            }
            saida << endl;
        }
    }
    return 0;
}