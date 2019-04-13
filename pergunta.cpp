#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    vector<string> expr;
    string linha;
    getline(cin, linha);
    string pergunta, expr1;
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
    for(int i=0; i<expr.size(); i++){
        cout << expr[i] << endl;
    }
    if(pergunta == "etautologia?"){
        cout << "Vai ser tauto" << endl;
    }
    else if(pergunta == "esatisfativel?"){
        cout << "Vai ser satisfa" << endl;
    }
    else if(pergunta == "erefutavel?"){
        cout << "Vai ser refuta" << endl;
    }
    else if(pergunta == "insatisfativel?"){
        cout << "Vai ser insatisfa" << endl;
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
        cout << "E consequencia logica" << endl;
        for(int i=0; i<expr.size(); i++){
            cout << expr[i] << endl;
        }
    }
    return 0;
}
