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
int main(){
    string expr;
    string p, q;
    int index;
    getline(cin, expr);
    cout << expr << endl;
    char operador = findOperator(expr, &index);
    cout << operador << endl;
    if(operador != '~'){
        p = getLeft(expr, index);
        q = getRight(expr, index);
        cout << p << endl;
        cout << q << endl;
    }
}